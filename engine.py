class Engine:
    def read_stdout():
        # command = 'myjoypad/./a.out'
        # this is so I don't trigger the memory leak in myjoypad while debugging
        command = 'myjoypad/./dummy.sh'
        process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
        while True:
            output = process.stdout.readline()
            # print(output.decode())
            if output == '' and process.poll() is not None:
                break
            # X
            elif output.decode().strip() == '0':
                pass
            # A
            elif output.decode().strip() == '1':
                pass
            # B
            elif output.decode().strip() == '2':
                pass
            # Y
            elif output.decode().strip() == '3':
                pass
            # Left Trigger
            elif output.decode().strip() == '4':
                pass
            # Right Trigger
            elif output.decode().strip() == '5':
                pass
            # Select Button
            elif output.decode().strip() == '8':
                pass
            # Start Button
            elif output.decode().strip() == '9':
                pass


        rc = process.poll()
        return rc
