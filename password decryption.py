import itertools

alphabet_base64 = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789/+'
alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_'
count_of_nulls = 0

def str_of_binaries(password):
    line = ''
    global count_of_nulls
    for i in range(len(password)):
        temp = str(bin(ord(password[i])))[2:]
        if len(temp) != 8:
            temp = '0' * (8 - len(temp)) + temp
            line += temp
    while len(line) % 6 != 0:
        line += '00000000'
        count_of_nulls += 1

    return line


def separate(line):
    sixes = []
    temp = ''
    for i in range(len(line)):
        temp += line[i]
        if (len(temp)) == 6:
            sixes.append(temp)
            temp = ''
    return sixes


def base64_to_ascii(sixes):
    decoded = ''
    for i in range(len(sixes) - count_of_nulls):
        decoded += alphabet_base64[int(sixes[i], 2)]
    decoded += count_of_nulls * '='
    return decoded


def check_alphabet(password):
    flag = False
    for i in range(len(password)):
        if password[i] not in alphabet_base64 and password[i] != '=':
            print("Some characters not in acceptable alphabet. Try again", end='\n')
            flag = True
            break
    return flag


def main():
    global count_of_nulls
    correct = False
    while not correct:
        encoded = input("Enter encoded password: ")
        if len(encoded) > 8:
            print("Encoded password must contain max 8 symbols (because decoded password's max length is 4). Try again", end='\n')
            continue
        elif check_alphabet(encoded):
            continue
        else:
            correct = True

    for i in range(len(alphabet)):
        potential = alphabet[i]
        decoded = base64_to_ascii(separate(str_of_binaries(potential)))
        if encoded == decoded:
            print(f"Decoded password: {potential}")
            return
        else:
            count_of_nulls = 0
    for i in range(len(alphabet)):
        for j in range(len(alphabet)):
            potential = alphabet[i] + alphabet[j]
            decoded = base64_to_ascii(separate(str_of_binaries(potential)))
            if encoded == decoded:
                print(f"Decoded password: {potential}")
                return
            else:
                count_of_nulls = 0
    for i in range(len(alphabet)):
        for j in range(len(alphabet)):
            for k in range(len(alphabet)):
                potential = alphabet[i] + alphabet[j] + alphabet[k]
                decoded = base64_to_ascii(separate(str_of_binaries(potential)))
                if encoded == decoded:
                    print(f"Decoded password: {potential}")
                    return
                else:
                    count_of_nulls = 0
    for i in range(len(alphabet)):
        for j in range(len(alphabet)):
            for k in range(len(alphabet)):
                for l in range(len(alphabet)):
                    potential = alphabet[i] + alphabet[j] + alphabet[k] + alphabet[l]
                    decoded = base64_to_ascii(separate(str_of_binaries(potential)))
                    if encoded == decoded:
                        print(f"Decoded password: {potential}")
                        return
                    else:
                        count_of_nulls = 0


main()

