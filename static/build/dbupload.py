#!/usr/bin/python
import requests
import datetime
from argparse import ArgumentParser

time = datetime.datetime.now().strftime("%d %b %Y %H:%M")

parser = ArgumentParser(description='Upload files to your Dropbox account via dbinbox.com')
parser.add_argument('-f', '--files', type=str, action='store', nargs='*', required=True)
parser.add_argument('-u', '--username', type=str, action='store', required=True)
parser.add_argument('-t', '--time', action="store_true")

args = parser.parse_args()
url = 'http://dbinbox.com/send/%s/' % args.username

for filepath in args.files:
	print "Processing '%s'.." % filepath
	filename = filepath.split('/')[-1:][0]
	if args.time:
		files = {'file': ("%s %s" % (time, filename), open(filepath, 'rb'))}
	else:
		files = {'file': (filename, open(filepath, 'rb'))}
	requests.post(url, files=files)
	print "Uploading '%s' done" % filepath