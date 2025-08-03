import sys

def splat_stdin(start, end):
    content = sys.stdin.buffer.read()
    data = bytearray(content)

    for i in range(start, end):
        if i >= len(data):
            break
        if data[i] not in b'\n\r\t ':
            data[i] = ord(b' ')

    sys.stdout.buffer.write(data)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} start end", file=sys.stderr)
        sys.exit(1)

    start = int(sys.argv[1])
    end = int(sys.argv[2])
    splat_stdin(start, end)
