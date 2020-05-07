#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
List libraries including owned, shared and group libraries.
"""

import urllib2
import urllib
import simplejson as json
import requests
import urlparse
import httplib, mimetypes


def test(token):
    # replace with your token
    print token;

    url = 'http://192.168.3.14:8000/api2/repos/'

    req = urllib2.Request(url)
    req.add_header('Authorization', 'Token ' + token)
    response = urllib2.urlopen(req)
    the_page = response.read()

    
    print the_page
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


    """ the_page = test(token);
    if "test" in the_page: 
        pass
    else:
        print('ERROR') """

    """ for num in range(1,100):
        the_page = test(token);
        if "test" in the_page: 
            pass
        else:
            print('ERROR')
            break  """

    return token

def get_upload_link(url, token):
    resp = requests.get(
        url, headers={'Authorization': 'Token {token}'. format(token=token)}
    )
    return resp.json()

if __name__=='__main__':
    url='http://192.168.3.14:8000/'
    username='111@saming.com'
    password='111'

    token= get_token(url,username,password)
    upload_link = get_upload_link(url+'api2/repos/a222363f-8bc8-4a13-bcfb-758cd199d455/upload-link/',token)
    response = requests.post(
        upload_link, data={'filename': 'ubuntu-14.04.5-desktop-amd64.iso', 'parent_dir': '/图片/'},
        files={'file': open('/home/hao/ubuntu-14.04.5-desktop-amd64.iso', 'rb')},
        headers={'Authorization': 'Token {token}'. format(token=token)}
    )
    print response
    if str(str(response).split('[')[1]).split(']')[0] == '200' :
        print "liushihao"
    else:
        print "Error"
    #print token


    