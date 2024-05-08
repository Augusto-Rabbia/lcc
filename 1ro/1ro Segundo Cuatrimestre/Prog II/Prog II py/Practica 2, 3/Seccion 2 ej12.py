def LogIn():
    Password = "pass123123"
    Attempts = 3
    Guess = ""
    while Guess != Password and Attempts != 0:
        Guess = input("Please enter the password (you have " + str(Attempts) + " attempts left): ")
        Attempts -= 1
    if Attempts != 0:
        return True
    else: return False
print(LogIn())