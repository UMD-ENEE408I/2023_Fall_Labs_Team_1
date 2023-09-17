while(True):
    n = int(input("Enter the number of rows: ").strip())
    if(n==-1):
        break
    for i in range (n+1):
        print(" "*(n-i)+"* " *i)