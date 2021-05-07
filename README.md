# linux-browser
### _Text based web browser supporting tabs, windows which extracts the links from the URL specified._

## Features

Users will be able to   
 - create a new tab ( multiple tabs residing in different threads )
 - See active tabs
 - Close particular tab
 - Go to particular tab

Users will be able to
- open new windows which can have multiple tabs- 
- Close particular window
- See active windows
- User can see what tabs other window is holding now and can run

## How to run

Program requires curl and jq installed in linux.

Install the dependencies

```sh
sudo apt-get install curl
sudo apt-get install jq
```

```sh 
sudo chmod 777 run_program.sh
```

```sh
cd src/
make clean && make
cd ../bin/
./main.out
```

## License

**Free Software, Hell Yeah!**
