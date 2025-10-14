salt = "01011010"

password = "2?665"
password = [format(ord(c), '08b') for c in password]

result = []
for n in range(len(password)):
    newn = ""
    for i in range(len(password[n])):
        j = int(password[n][i])
        k = int(salt[i])
        newn += str((j+k)%2)
    result.append(newn)

# Brings the binary back to ascii
password = ''.join(chr(int(b, 2)) for b in result)

print(password)