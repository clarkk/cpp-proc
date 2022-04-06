# cpp-proc
High performance (Linux) process status information tool. It reads the dynamic `/proc` filesystem directly in the same way as `ps`, but with much better performance (2-3 times faster).

It can also be used with the built-in regex argument `-grep`. It will then work in the same way as if you pipe the output like `ps ... | grep ...` but again with much higher performance because the regex is performed directly and internal on the result before output.

The output information is separated by whitespaces and very easy to read and parse.

### Example
```
# ./proc -name php -grep "cronjob\.php listen_websockets" -stat
8650 8648 0% 39.7M 1649252760 2847 php /var/www/php/cronjob.php listen_websockets
15272 15271 0.3% 38.9M 1649255581 26 php /var/www/php/cronjob.php listen_websockets
```

## -name (argument)
The `-name <name>` filter all processes by name. If you want to see all PHP processes use `-name php`.

## -grep (argument)
The `-grep <pattern>` argument can be used to run the result through a regex pattern before output in the same way as piping through `grep`.

## -stat (argument)
The `-stat` argument prints extended information about CPU and memory usage, which the `ps` command can't do natively. These information are calculated accross multiple files in the `/proc` file system.

## -help (argument)
```
# ./proc -help
proc: version 0.1

Output process list
PID PPID CMD

Output process list with -stat
PID PPID CPU MEM STARTTIME EXECTIME CMD

Usage: proc [options]
Options:
        -name <name>         -- Process name
        -grep <pattern>      -- Filter output grep alike
        -stat                -- Show CPU/mem
        -help
```
