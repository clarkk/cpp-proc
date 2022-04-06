# cpp-proc
High performance (Linux) process status information tool. It reads the dynamic `/proc` filesystem directly in the same way as `ps`, but with much better performance (2-3 times faster).

It can also be used with the built-in regex argument `-grep`. It will then work in the same way as if you pipe the output like `ps ... | grep ...` but again with much higher performance because the regex is performed directly on the result before output.

### Example
`./proc -name php -grep "cronjob\.php listen_websockets" -stat`

## -name (argument)
The `-name <name>` filter all processes by name. If you want to see all PHP processes use `-name php`.

## -grep (argument)
The `-grep <pattern>` argument can be used to run the result through a regex pattern before output in the same way as piping through `grep`.

## -stat (argument)
The `-stat` argument prints extended information about CPU and memory usage, which the `ps` command can't do natively. These information is calculated accross multiple files in the `/proc` file system.

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
