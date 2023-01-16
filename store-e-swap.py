import time

events = [1, 2, 3, 4, 5]
buffer = [None, None]

cnt = 0

def swap(buffer):
	temp = buffer[0]
	buffer[0] = buffer[1]
	buffer[1] = temp
	# return buffer


def get_new_buffer(event):
	if buffer[0] == None:
		buffer[0] = event
	elif (buffer[0] != None) and (buffer[1] == None):
		buffer[1] = event
	elif (buffer[0] != None) and buffer[1] != None:
		swap(buffer)
		buffer[0] = event
	print(buffer)
	time.sleep(1)


while True:
	cnt += 1
	# buffer.append(cnt)
	if buffer[0] == None:
		buffer[0] = cnt
	elif (buffer[0] != None) and (buffer[1] == None):
		buffer[1] = cnt
	elif (buffer[0] != None) and buffer[1] != None:
		swap(buffer)
		buffer[0] = cnt
	print(buffer)
	time.sleep(1)