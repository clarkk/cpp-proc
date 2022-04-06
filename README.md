# cpp-proc
High performance (Linux) process status information tool. It reads the dynamic `/proc` kernel filesystem directly in the same manner as the native Linux command `ps`, but with much better performance (2-3 times faster).

It is shipped with a built-in regex option `-grep`, and will emulate piping `ps ... | grep ...`, but with much higher performance. The regex logic is implemented in the internal code and applied on the result before output without unnecessary overhead.

The output information is separated by whitespaces and parsed with convenience.

### Example
```
# ./proc -name php -grep "cronjob\.php listen_websockets" -stat
8650 8648 0% 39.7M 1649252760 2847 #php /var/www/php/cronjob.php listen_websockets
15272 15271 0.3% 38.9M 1649255581 26 #php /var/www/php/cronjob.php listen_websockets
```

## -name <name>
The `-name` filter all processes by name. If you want to list all PHP processes use `-name php`.

## -grep <pattern>
The `-grep` filter the result by a regex pattern before output in the same manner as piping `ps ... | grep ...`.

## -stat
The `-stat` option appends information about CPU and memory usage. This information is calculated across several files in the `/proc` file system, and CPU time is calculated as an average since the process was created.

## -help
```
# ./proc -help
proc: version 0.1

Output process list
PID PPID #CMD

Output process list with -stat
PID PPID CPU MEM STARTTIME EXECTIME #CMD

Usage: proc [options]
Options:
        -name <name>         -- Process name
        -grep <pattern>      -- Filter output grep alike
        -stat                -- Show CPU/mem
        -help
```