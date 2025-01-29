# How to run

## Complie all source files
```
gcc main.c util.c dir.c -o shell
```

## Run shell
```
./shell
```

# Supported Commands

-  `cd [DIR_NAME | ABSOLUTE_PATH]`: Changes directory
    ```
    DIR_NAME | ABSOLUTE_PATH: optional
    ```
  
-  `dir [DIR_NAME] [FLAGS]`: Lists items in direcotry
   ```
   DIR_NAME: optional
   FLAGS:
       -a: List hidden files and folders
   ```

-  `ls [DIR_NAME] [FLAGS]`: Lists items in direcotry with colors
   ```
   DIR_NAME: optional
   FLAGS:
       -a: List hidden files and folders
   ```

-  `mkdir [DIR_NAME]`: Creates a directory
   ```
   DIE_NAME: required
   ```

-  `env | environ`: Lists all environment variables

-  `set [KEY] [VALUE]`: Sets enviroment variable
   ```
   KEY: required
   VALUE: required
   ```

-  `echo [MSG]`: Prints message to terminal
   ```
   MSG: required
   $ENV: prints env var
   ```

-  `&`: Runs proccess in backround
-  `>`: Write output to file
-  `>>`: Appends output to file
-  `<`: Get input from file

-  External commands are supported by forking
    
