import sys

def splat_stdin(start, end):
    content = sys.stdin.buffer.read()
    data = bytearray(content)
    out = bytearray()

    for i in range(start, end):
        if i >= len(data):
            break
        out.append(data[i])

    sys.stdout.buffer.write(out)
    print()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} start end", file=sys.stderr)
        sys.exit(1)

    start = int(sys.argv[1])
    end = int(sys.argv[2])
    splat_stdin(start, end)
