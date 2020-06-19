#!/usr/bin/python
# -*- coding: UTF-8 -*-
def get_disk_info(self):

      '''
      function:       get_disk_info()
      return values:  return a dic data 
      json_data=  {
                      'DiskNumber': str(j),
                      'Size': Size,
                      'Used': Used,
                      'UsedPercent': UsedPercent,
                      'Rest': Rest
                    }
      author:    jims hou 
      date:     2018/4/14
      '''

      Disklist = []
      DiskInfoDic = {}

      #set disk content size
      Disk_Content_Size=100
      j = 0
      try:
          Diskinfolist = str(os.popen(['df', '-h'], stdout=subprocess.PIPE, shell=True).communicate()).split('\\n')
      except Exception as e:
          print "error"
          return
      for i in Diskinfolist:
          if i.startswith('/dev/sd'):
              info = i.split()
              Size = float(round(float(info[1]) / 1048576, 2))
              if Size > Disk_Content_Size:
                  Size = str(Size) + 'G'
                  Used = float(round(float(info[2]) / 1048576, 2))
                  Used = str(Used) + 'G'
                  UsedPercent = info[4]
                  Rest = float(round(float(info[3]) / 1048576, 2))
                  Rest=str(Rest)+'G'
                  DiskInfoDic = {
                          'DiskNumber': str(j),
                          'Size': Size,
                          'Used': Used,
                          'UsedPercent': UsedPercent,
                          'Rest': Rest
                      }
                  Disklist.append(DiskInfoDic)
                  j += 1
      disk_info_dic = {'DiskInfo': Disklist}
      return disk_info_dic
