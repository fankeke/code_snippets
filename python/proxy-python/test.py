#!/usr/bin/env python
key='123456'

import sys,os,hashlib,struct
import string


md5=hashlib.md5()
md5.update(key)
s=md5.digest()
(a,b)=struct.unpack('<QQ',s)


for i in range(1,20):
	print(i)



