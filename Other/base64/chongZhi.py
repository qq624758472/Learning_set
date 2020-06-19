# -*- coding: utf-8 -*-
import requests
import os,sys

#给100002 到 100099  集体发送邮件。
id = 100002


#账号id,  充值的东西，   这个东西的数量。
def send_post_request(type, id, item, num):
    url = "http://106.52.97.201/gm/user/gmquery.php"

    payload = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"type\"\r\n\r\n" + type + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"uid\"\r\n\r\n" +str(id)+ "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"item\"\r\n\r\n" + str(item) + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"num\"\r\n\r\n"+ str(num) + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"qu\"\r\n\r\n1\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"checknum\"\r\n\r\nqwe\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--"
    headers = {
        'content-type': "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW",
        'User-Agent': "PostmanRuntime/7.19.0",
        'Accept': "*/*",
        'Cache-Control': "no-cache",
        }

    response = requests.request("POST", url, data=payload, headers=headers)

    print(response.text)


def chong_zhi_yuan_bao(type, id, item, checknum):
    url = "http://106.52.97.201/gm/user/gmquery.php"

    payload = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"type\"\r\n\r\n" + type + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"uid\"\r\n\r\n" + str(id) + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"item\"\r\n\r\n" + str(item) + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"f\"\r\n\r\nfalse\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"qu\"\r\n\r\n1\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"checknum\"\r\n\r\nqwe\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--"
    headers = {
        'content-type': "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW",
        'User-Agent': "PostmanRuntime/7.19.0",
        'Accept': "*/*",
        'Cache-Control': "no-cache",
    }

    response = requests.request("POST", url, data=payload, headers=headers)

    print(response.text)


# #道具发送完成
# for num in range(100002,100099):
#     send_post_request("mail",num, 93049, 999)

#send_post_request("mail" ,100002, 93049, 1)


#93016 喜糖
#93025 金宝箱
#93049 炼化锁


#元宝充值
# recharge 充值元宝，   mail  发送邮件道具。
#chong_zhi_yuan_bao("recharge" ,100002, 114, 'qwe')

#10元冲1500w
# for num in range(1,10):
#     chong_zhi_yuan_bao("recharge" , 100002 , 114, 'qwe')

# for num in range(100002,100099):
#     chong_zhi_yuan_bao("recharge" ,num, 114, 'qwe')




if __name__ == '__main__':
    id = sys.argv[1]
    #chong_zhi_yuan_bao("recharge", id, 114, 'qwe')
    for num in range(100002,100399):
        chong_zhi_yuan_bao("recharge" ,num, 114, 'qwe')

