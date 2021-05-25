import argparse
def main():
    print('hello world')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='')
    #parser.add_argument('')
    args = parser.parse_args()
    
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
