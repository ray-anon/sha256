import hashlib

def sha256_string(text):
    return hashlib.sha256(text.encode('utf-8')).hexdigest()

def read_cpp_hash(path):
    with open(path, 'r') as f:
        return f.read().strip()

original_text = input("Enter your password: ")

cpp_hash = read_cpp_hash("test.txt")

python_hash = sha256_string(original_text)

print(f"Python SHA-256: {python_hash}")
print(f"C++ SHA-256:    {cpp_hash}")

if cpp_hash.lower() == python_hash.lower():
    print("Hashes match")
else:
    print("Hashes do not match")
