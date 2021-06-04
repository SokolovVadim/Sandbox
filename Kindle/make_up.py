import sys
def process_text(filename):
	lines = []
	with open(filename) as f:
		lines = f.readlines()
	new_text = []
	for i in range(len(lines) - 1):
		if (lines[i][-1] == '\n') and (lines[i + 1][0] != '\n'):
			new_text.append(lines[i][:len(lines[i]) - 1])
		else:
			new_text.append(lines[i])

	text = ''.join(new_text)
	return text

def write_to_file(filename, modified_data):
    name = ''
    for i in range(len(filename)):
        if filename[i] == '.':
            name = filename[:i]
            break
    
    with open(name + '_processed.txt', 'w') as x_file:
        x_file.write('{}'.format(modified_data))

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('type file name!')
        exit(1)
    filename = sys.argv[1]
    modified_data = process_text(filename)
    write_to_file(filename, modified_data)
