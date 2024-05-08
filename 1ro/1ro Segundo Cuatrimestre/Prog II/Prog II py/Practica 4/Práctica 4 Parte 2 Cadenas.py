def backwards(Str):
    return Str[::-1]

print(backwards("hello how are you"))

#ej 2

def count(Str,Char):
    Count = 0
    for i in Str:
        if i == Char:
            Count += 1
    return Count

print(count("hello how are you","h"))

#ej 3

def how_many_vowels(Str):
    Count = 0
    for i in Str:
        if i.lower() in "aeiou":
            Count += 1
    return Count

print(how_many_vowels("HellO hOw are yoU"))

def long_words_count(Str):
    Count = 0
    Words = Str.split(" ")
    for i in Words:
        if len(i) > 5:
            Count += 1
    return Count

print(long_words_count("looong wooords short words"))