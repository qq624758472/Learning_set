#!/usr/bin/env python
# -*- coding: UTF-8 -*-
import urllib2
import urllib
import requests
import os
import urlparse
import httplib, mimetypes
import time,datetime
from requests_toolbelt import  *
import simplejson as json


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

#��ȡ���Ͽ���Ϣ
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


def my_callback(monitor):
    # Your callback function
    print monitor.bytes_read


#open(file_posion.decode('utf-8').encode('gb18030')
def upload_file(url,username,passwd,data_lib_id,filename,parent_dir,file_posion):
    token = get_token(url,username,passwd)
    upload_link = get_upload_link(url+'api2/repos/'+data_lib_id+'/upload-link/',token)

    urlparts = urlparse.urlsplit(upload_link[1:-1])

    print token
    print upload_link

    # try:
    #     files={'file': open(file_posion, 'rb')}
    # except:
    #     print 'Error!'
    # response = requests.post(
    #     upload_link, data={'filename': filename, 'parent_dir': parent_dir},
    #     files=files,
    #     headers={'Authorization': 'Token {token}'. format(token=token)}
    # )
    # print response

#--------------------------------------------------------------------------------
    print filename
    print parent_dir
    print file_posion
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

    r = requests.post(upload_link, data=m, headers=req_headers) 
    print r

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


# passwd.conf /home/pi/work/conf/passwd.conf
if __name__=='__main__':
    url='http://192.168.6.24:8000/'
   
    passwd = '123456'
    username='lxm@saming.com'
    #data_lib_id='ac2e8564-ca86-4c78-b9c7-f72b0b4b3d03'
    data_lib_id ='82ff3a3d-7080-4513-8d12-5f5bef0ee52a'
    
    # filename_utf8 = os.path.join(root,file)
    # suffix = filename_utf8.split(".")[-1]  #
    # filename = filename_utf8.decode('utf-8')

    filename_utf8 = '/home/pi/work/mnt/disk4/高配瘦终端配置信息.txt'
    file_utf8 = '高配瘦终端配置信息.txt'
    mulu_utf8 = '/图片'
    mulu = mulu_utf8.decode('utf-8')
    filename = filename_utf8.decode('utf-8')
    file = file_utf8.decode('utf-8')
    upload_file(url,username,passwd,data_lib_id,file,'/图片',filename)
