#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import urllib2
import urllib
import requests
import os
import urlparse
import httplib, mimetypes
import time,datetime
import write_datalib
from requests_toolbelt import  *
import simplejson as json

last_schedule = 0

def get_data_lib_for_token(url,token):
    # replace with your token
    url = url + "api2/repos/"

    req = urllib2.Request(url)
    req.add_header('Authorization', 'Token ' + token)
    response = urllib2.urlopen(req)
    the_page = response.read()

    #print the_page
    return the_page     

def get_token(url,username,password):
    '''
    acquire token
    '''  
    url = '%s/api2/auth-token/'%url
    values = {'username': username,
          'password': password}
    data = urllib.urlencode(values)
    req = urllib2.Request(url, data)
    response = urllib2.urlopen(req)
    the_page = response.read()
    token = json.loads(the_page)['token']
    return token

#获取资料库信息
def get_data_lib(url,username,passwd):
    token = get_token(url,username,passwd)
    the_page = get_data_lib_for_token(url,token)
    return the_page


def read_file(path):
    with open(path, "r")as f:
        data=f.read()
    return data


def log(import_log):
    cmd = "echo %s   >> /home/pi/work/import/import.log" % import_log
    os.system(cmd)


def get_upload_link(url, token):
    resp = requests.get(
        url, headers={'Authorization': 'Token {token}'. format(token=token)}
    )
    return resp.json()

def get_content_type(filename):
    return mimetypes.guess_type(filename)[0] or 'application/octet-stream'

def encode_multipart_formdata(fields, files):
    """
    fields is a sequence of (name, value) elements for regular form fields.
    files is a sequence of (name, filename, value) elements for data to be uploaded as files
    Return (content_type, body) ready for httplib.HTTP instance
    """
    BOUNDARY = '----------ThIs_Is_tHe_bouNdaRY_$'
    CRLF = '\r\n'
    L = []
    for (key, value) in fields:
        L.append('--' + BOUNDARY)
        L.append('Content-Disposition: form-data; name="%s"' % key)
        L.append('')
        L.append(value)
    for (key, filename, value) in files:
        L.append('--' + BOUNDARY)
        L.append('Content-Disposition: form-data; name="%s"; filename="%s"' % (key, filename))
        L.append('Content-Type: %s' % get_content_type(filename))
        L.append('')
        L.append(value)
    L.append('--' + BOUNDARY + '--')
    L.append('')
    body = CRLF.join(L)
    content_type = 'multipart/form-data; boundary=%s' % BOUNDARY
    return content_type, body


def post_multipart(scheme, host, port, selector, fields, files):
    """
    Post fields and files to an http host as multipart/form-data.
    fields is a sequence of (name, value) elements for regular form fields.
    files is a sequence of (name, filename, value) elements for data to be uploaded as files
    Return the server's response page.
    """
    content_type, body = encode_multipart_formdata(fields, files)
    if scheme and scheme.lower() == "http":
        h = httplib.HTTP(host, port)
    else:
        h = httplib.HTTPS(host, port)
    h.putrequest('POST', selector)
    h.putheader('content-type', content_type)
    h.putheader('content-length', str(len(body)))
    h.endheaders()
    h.send(body)
    errcode, errmsg, headers = h.getreply()
    print errcode, errmsg, headers
    return h.file.read()


# 字节bytes转化kb\m\g
def formatSize(bytes):
    try:
        bytes = float(bytes)
        kb = bytes / 1024
    except:
        print "formatSize Error"
        log("formatSize Error")
        return 0.0

    if kb >= 1024:
        M = kb / 1024
        if M >= 1024:
            G = M / 1024
            return "%.1f" % (G)
        else:
            return "%.1f" % (M / 1024)
    else:
        return 0.0


def my_callback(monitor):
    # Your callback function
    print monitor.bytes_read

def get_file_size(file_posion):
    try:
        size = os.path.getsize(file_posion)
        return formatSize(size)
    except Exception as err:
        return 0.0

#need pip install requests_toolbelt
#open(file_posion.decode('utf-8').encode('gb18030')
def upload_file(url,username,passwd,data_lib_id,filename,parent_dir,file_posion):
    file_size = get_file_size(file_posion)

    token = get_token(url,username,passwd)
    file_size_str = str(file_size)
    upload_link = get_upload_link(url+'api2/repos/'+data_lib_id+'/size/'+file_size_str+'/upload-link/',token)

    print 'upload_link is :[%s]'%upload_link
    #判断返回值是不是over_size  是的话，硬盘容量不足，返回0
    if upload_link == 'over_size':
        return 0

    #urlparts = urlparse.urlsplit(upload_link[1:-1])

    # files={'file': open(file_posion, 'rb')}
    # response = requests.post(
    #     upload_link, data={'filename': filename, 'parent_dir': parent_dir},
    #     files=files,
    #     headers={'Authorization': 'Token {token}'. format(token=token)}
    # )
    # print token
    # print upload_link

    m = MultipartEncoder(
        fields={
            'filename': filename,
            'parent_dir': parent_dir,
            'file':(filename, open(file_posion, 'rb'), 'application/octet-stream')
            },
            boundary='---------------------------7de1ae242c06ca'
        )
    m = MultipartEncoderMonitor(m, my_callback)
    req_headers = {'Content-Type': m.content_type,
               'Authorization': 'Token {token}'. format(token=token)}

    response = requests.post(upload_link, data=m, headers=req_headers) 
    print response

    if str(str(response).split('[')[1]).split(']')[0] == '200' :
        return 1
    else:
        log("upload [%s] Error!" % file_posion)
        return -1


def read_json_data(mount_posion):
    with open("/home/pi/work/disk-init/diskinfo.conf", "r")as f:
        data=f.read()
    json_data=json.loads(data)['DiskInfo']
    if json_data == None:
        print 'json_data is NULL!'
        return -1
    for disk in json_data:
       dir = ''.join(disk["mount_posion"].split())
       #print dir
       if dir == mount_posion:
           return ''.join(disk["dev_name"].split())
    return -1


def last_schedule_setting():
    global last_schedule
    if 100 - last_schedule > 5:
        while(1):
            last_schedule = last_schedule + 3
            if last_schedule >= 100:
                break
            write_datalib.write_datalib(username,backup_source,backup_target ,'10', last_schedule)
            time.sleep(2)



#判断要导出的硬盘是否被拔掉
def if_disk_be_pulled(dev_name):
    cmd = "fdisk -l"
    r = os.popen(cmd)       #执行该命令
    info = r.readlines()    #读取命令行的输出到一个list
    r.close()
    #print info
    for line in info:       #按行遍历
        try:
            #print line.split()[0]
            if line.split()[0] == dev_name:
                #print '111111111111111111111111'
                return 1
        except:
            pass
    return -1


#导入功能：
# 1.导入功能需要先获取资料库信息（需要用户名和密码），然后让用户选择需要导入的数据盘符和对应的资料库。
# 2.然后遍历指定的盘符，分析文件后缀，然后上传到指定文件夹中。
# passwd.conf在/home/pi/work/conf/passwd.conf
if __name__=='__main__':
    global last_schedule
    
    url='http://127.0.0.1:8000/'

    cmd = "echo %s   > /home/pi/work/import/import.log" % "Start import Data!"
    os.system(cmd)

    ##判断上传的是共享资料库还是私人资料库。  1.私人资料库，    2.共享资料库
    try:
        file_tmp = open("/home/pi/work/conf/share_or_person.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/share_or_person.conf Error!")
        exit()
    share_or_person = file_tmp.read() 
    


    try:
        file_tmp = open("/home/pi/work/conf/passwd.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    passwd = file_tmp.read()   #用户密码


    try:
        file_tmp = open("/home/pi/work/conf/username.conf",'r')
    except IOError:
        log("open /home/pi/work/conf/passwd.conf Error!")
        exit()
    username=file_tmp.read()   #用户名
    print "passwd [%s]\n"%passwd
    print "username [%s]\n"%username
    print "share_or_person [%s]\n"%share_or_person

    #获取资料库信息.   需要私有资料库ID 和 共享资料库ID
    list_data_lib_info = []#资料库名，对应的ID
    data_lib = get_data_lib(url,username,passwd)


    json_data_lib=json.loads(data_lib)
    for disk in json_data_lib:
        #print disk["name"]
        data_lib_name = disk["name"]
        #log(disk["id"])
        data_lib_id = disk["id"]
        data_lib_info = {
                    'data_lib_name': data_lib_name,
                    'data_lib_id': data_lib_id,
                    }
        if data_lib_info['data_lib_name'] == u"私人资料库" or data_lib_info['data_lib_name'] == u"共享资料库":
            #print "data_lib_info is [%s]"%data_lib_info['data_lib_name']
            #log("data_lib_info is [%s]"%data_lib_info['data_lib_name'])    #log  中含中文直接报错退出
            list_data_lib_info.append(data_lib_info)

    
    if share_or_person == '1':  # 1.私人资料库
        for lib_name in list_data_lib_info:
            if lib_name['data_lib_name'] == u'私人资料库':
                data_lib_id = lib_name['data_lib_id']
    elif share_or_person == '2':  #2.共享资料库
        for lib_name in list_data_lib_info:
            if lib_name['data_lib_name'] == u'共享资料库':
                data_lib_id = lib_name['data_lib_id']
    else:
        data_lib_id = -1
        log("NOT FOUND DATALIB")
        exit()

    
    #遍历用户插入的文件夹
    try:
        mount_posion = dir = read_file("/home/pi/work/conf/mount_posion.conf")
    except:
        log("read_file[/home/pi/work/conf/mount_posion.conf] Error!")
        exit()

    
    dev_name = read_json_data(mount_posion)
    if dev_name == -1:
        log('read_json_data return None!')
        exit()

    #取出挂载位置对应的是第几个分区。
    backup_source = dev_name
    backup_target = share_or_person
    
    #print "--->%s,  %s"%(mount_posion,dev_name)
    picture_suffix_list=["flic","FLIC","emf","EMF","hdri","HDRI","cdr","CDR","pcd","PCD","dxf","DXF","ufo","UFO","eps","EPS","ai","AI","wmf","WMF","tif","tiff","jpg","jpeg","gif","png","bmp","ico","heic","psd","raw","TIF","TIFF","JPG","JPEG","GIF","PNG","BMP","ICO","HEIC","PSD","RAW","webp","WEBP","pcx","PCX","tga","TGA","exif","EXIF","fpx","FPX","svg","SVG"]
    music_suffix_list=["ra","RA","tak","TAK","rmi","RMI","ogx","OGX","mpc","MPC","mpa","MPA","mp5","MP5","mp2","MP2","mka","MKA","midi","MIDI","mid","MID","m1a","M1A","m2a","m4a","M4A","M2A","eac3","EAC3","dtshd","DTSHD","dts","DTS","au","AU","amr","AMR","aiff","AIFF","aif","AIF","ac3","AC3","wv","WV","tta","TTA","cda","CDA","ram","RAM","mp3","wav","wma","ogg","ape","flac","aac","m4a","MP3","WAV","WMA","OGG","APE","FLAC","AAC","M4A"]
    documents_suffix_list=["doc","docx","xls","xlsx","ppt","pptx","txt","pdf","wps","log","conf","py","c","cpp","java","h","js","sh","DOC","DOCX","XLS","XLSX","PPT","PPTX","TXT","PDF","WPS","LOG","CONF","PY","C","CPP","JAVA","H","JS","SH"]
    video_suffix_list=["m1v","wtv","WTV","webm","WEBM","vro","VRO","vp6","vp7","VP6","VP7","vdat","VDAT","tod","TOD","ogv","OGV","ogm","OGM","nsr","NSR","M1V","nsv","NSV","mxf","MXF","mtv","MTV","mp4v","MP4V","mod","MOD","k3g","K3G","ivm","IVM","h264","H264","evo","EVO","dv","DV","dpg","DPG","divx","DIVX","bik","BIK","amv","AMV","264","3gpp","3GPP","3gp2","3GP2"," 3gp","3GP","3g2","3G2","ts","TS","tp","TP","pva","PVA","mpv2","MPV2","mpeg1","MPEG1","mpeg2","MPEG2","mpeg4","MPEG4","mpe","MPE","mp2v","MP2V","m4p","M4P","m4b","M4B","m2t","M2T","m2ts","M2TS","m2v","M2V","m2p","M2P","Asf","asf","ASF","qt","QT","f4v","F4V","hlv","HLV","swf","SWF","wm","WM","wmv","WMV","dvr-ms","DVR-MS","rm","rmvb","mpg","mpeg","3gp","mov","mp4","m4v","avi","dat","mkv","flv","vob","RM","RMVB","MGP","MPEG","3GP","MOV","MP4","M4V","AVI","DAT","MKV","FLV","VOB"]
    
    write_datalib.write_datalib(username,backup_source,backup_target ,'10', 0)
    file_number = 0
    try:
        for root, dirs, files in os.walk(dir):
            file_number = file_number + len(files)
    except Exception as e:
        log("list dir Error!")
        exit()

    if file_number == 0:
        file_number=j=i=100
    else:
        j = i = float(100)/float(file_number) #一个文件所占百分比。
    
    last_schedule = 1
    try:
        #print dir
        for (root, dirs, files) in os.walk(dir):
            value = -1
            #print files
            for fil in files:
                value = -1
                try:
                    #print fil
                    file_tr = fil.decode('utf-8')
                    filename_utf8 = os.path.join(root,fil)
                    try:
                        suffix = filename_utf8.split(".")[-1]  #-1为最后一个  suffix后缀
                    except:
                        print "wuhouzhui"
                    filename = filename_utf8.decode('utf-8')
                    if suffix in picture_suffix_list:   #图片后缀
                        print "file is .gif"
                        try:
                            value = upload_file(url,username,passwd,data_lib_id,file_tr,'/图片/',filename)
                            if value == 0:
                                log('over_size Error')
                                break
                        except:
                            try:
                                log("upload_file [%s] Error!"% filename)
                                print "upload_file [%s] Error!"% filename
                            except:
                                pass
                    elif suffix in music_suffix_list:   #音乐后缀
                        print "file is .mp3"
                        try:
                            value = upload_file(url,username,passwd,data_lib_id,file_tr,'/音乐/',filename)
                            if value == 0:
                                log('over_size Error')
                                break
                        except:
                            try:
                                log("upload_file [%s] Error!"% filename)
                                print "upload_file [%s] Error!"% filename
                            except:
                                pass
                    elif suffix in documents_suffix_list: #文档后缀
                        print "file is .txt"
                        try:
                            value = upload_file(url,username,passwd,data_lib_id,file_tr,'/文档/',filename)
                            if value == 0:
                                log('over_size Error')
                                break
                        except:
                            try:
                                log("upload_file [%s] Error!"% filename)
                                print "upload_file [%s] Error!"% filename
                            except:
                                pass
                    elif suffix in video_suffix_list:      #视频后缀
                        print "file is .mp4"
                        try:
                            value = upload_file(url,username,passwd,data_lib_id,file_tr,'/视频/',filename)
                            if value == 0:
                                log('over_size Error')
                                break
                        except:
                            try:
                                log("upload_file [%s] Error!"% filename)
                                print "upload_file [%s] Error!"% filename
                            except:
                                pass
                    else:
                        print "file is QiTa"
                        try:
                            value = upload_file(url,username,passwd,data_lib_id,file_tr,'/其他/',filename)
                            if value == 0:
                                log('over_size Error')
                                break
                        except:
                            try:
                                log("upload_file [%s] Error!"% filename)
                                print "upload_file [%s] Error!"% filename
                            except:
                                pass
                    j= j + i
                    #print "----------->j=[%d],i=[%d]"%(j,i)
                    if j> 100:
                        j = 100

                    # #判断用户是否拔掉硬盘，是的话直接写入数据库报错，退出。
                    # if if_disk_be_pulled(dev_name) == -1:
                    #     write_datalib.write_datalib('-1','-1','-1','-1',-1)
                    #     write_datalib.write_operating_history_datalib(username,'10',-1,import_time,share_or_person,dev_name)
                    #     os._exit(0) #结束整个进程  0正常退出  1抛出异常退出。
                        
                    #和上次进度一样时，不用在次写入数据库。
                    if last_schedule == int(j):
                        continue
                    write_datalib.write_datalib(username,backup_source,backup_target ,'10', j)
                    last_schedule = j
                except:
                    print 'xunhuan Error'
            
            if value == 0:
                break
    except Exception as e:
        #print "import Error!"
		log("import Error!")
    import_time  = datetime.datetime.now().strftime('%Y.%m.%d %H:%M:%S')

    #判断用户是否拔掉硬盘，是的话直接写入数据库报错，退出。写两遍的原因是因为拔掉硬盘后上边的循环会迅速跑完直接错过退出函数。
    if if_disk_be_pulled(dev_name) == -1:
        write_datalib.write_datalib('-1','-1','-1','-1',-1)
        write_datalib.write_operating_history_datalib(username,'10',-1,import_time,share_or_person,dev_name)
        os._exit(0) #结束整个进程  0正常退出  1抛出异常退出。
    if value == 0:
        write_datalib.write_datalib('-1','-1','-1','-1',-1)
        write_datalib.write_operating_history_datalib(username,'10',-1,import_time,share_or_person,dev_name)
        log('Disk allready full!')
    else:
        last_schedule_setting() 
        write_datalib.write_datalib(username,backup_source,backup_target ,'10',100) #导入失败 ,1,拔盘。 2.容量不足。
        write_datalib.write_operating_history_datalib(username,'10',100,import_time,share_or_person,dev_name)
        time.sleep(9)
        write_datalib.write_datalib('-1','-1','-1','-1', -1)
        log('import success!')
    os._exit(0)
    
