import getpass

def main():
    print("Welcome to the Prayer Room. Please enter your username: ")
    username: str = input()
    # search for user in db
    password: str = getpass.getpass(f"Hello, {username}! Please enter your password: ")
    #verify password
    
    print("Program finished.")

if __name__ == "__main__":
    main()