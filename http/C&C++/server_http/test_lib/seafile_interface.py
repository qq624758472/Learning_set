#!/usr/bin/env python
# -*- coding: utf-8 -*-

import urllib
import urllib2
import simplejson as json
import requests

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


def get_upload_link(url, token):
    
    '''
    using url and token paramaters to acquire upload link
    '''
    
    resp = requests.get(
        url, headers={'Authorization': 'Token {token}'. format(token=token)}
    )
    #print "url is [%s]!"%url
    return resp.json()



def get_download_link(url,token):
    
    '''
    using url and token paramaters to acquire download link
    '''
    
    resp = requests.get(
        url, headers={'Authorization': 'Token {token}'. format(token=token)})
    return resp.json()


def get_create_link(url,token):
    
    '''
    using url and token paramaters to acquire download link
    '''
    
    resp = requests.get(
        url, headers={'Authorization': 'Token {token}'. format(token=token)})
    return resp.json()



def upload_file(url,username,password,upload_url,file_dir):
    token= get_token(url,username,password)
    upload_link=get_upload_link(upload_url,token)
    print "upload_link is {%s}"%upload_link
    with open('/home/pi/work/upload_link.txt','w')as f:
        f.write(upload_link)
    try:
        response = requests.post(
            upload_link, data={'filename': '', 'parent_dir': '/'},
            files={'file': open(file_dir, 'rb')},
            headers={'Authorization': 'Token {token}'. format(token=token)})
        print 'upload successful'
    except:
        print 'upload fail'
    return response



def download_file(url,username,password,download_url,file_dir):
    token= get_token(url,username,password)
    try:
      download_link=get_download_link(download_url,token)
    except:
        print 'get_download_link error'
        return
    return download_link


def create_file(url,username,password,create_url,file_name):
    token= get_token(url,username,password)
    values={'operation':'create'}
    data=urllib.urlencode(values)
    try:
       response = requests.post(
             create_url,data,
             files={'file': open(file_name, 'wb')},
             headers={'Authorization': 'Token {token}'. format(token=token)})
       print 'create successful'
    except Exception as e:
        print 'create fail '+str(e)

if __name__=='__main__':
    url='http://192.168.3.14:8000/'
    username='111@saming.com'
    password='111'
    upload_file_url='http://192.168.3.14:8000/api2/repos/a222363f-8bc8-4a13-bcfb-758cd199d455/upload-link/'
    upload_file_dir='/home/pi/work/test_lib/test.py'
    download_file_url='http://192.168.6.17:8000/api2/repos/dc00d2c7-382f-4c43-bfa3-3a0426e5ce19/file/?p=/void.mp4&reuse=1'
    download_file_dir='/home/pi/script/void.mp4'
    create_file_url='http://192.168.6.17:8000/api2/v2.1/repos/dc00d2c7-382f-4c43-bfa3-3a0426e5ce19/file/?p=/'
    file_name='new.py'
##    download_link=download_file(url,username,password,download_file_url,download_file_dir)
##    print download_link
    upload_file(url,username,password,upload_file_url,upload_file_dir)
##    create_file(url,username,password,create_file_url,file_name)