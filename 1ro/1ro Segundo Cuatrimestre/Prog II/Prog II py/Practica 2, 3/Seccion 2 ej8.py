while True:
    A = input("Enter a positive number: ")
    if not(A.isnumeric()) or float(A) < 0:
        print("Invalid data type, enter a positive number.")
        pass
    else: break
