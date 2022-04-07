# cpp-proc
High performance (Linux) process status information tool. It reads the dynamic `/proc` kernel file system directly in the same manner as the native Linux command `ps`, but with much better performance (2-3 times faster).

It is shipped with a built-in regex option `-grep`, and will emulate piping `ps ... | grep ...`, but with much higher performance. The regex logic is implemented directly in the process iteration loop and applied on the result before output without unnecessary overhead.

The output information is separated by whitespaces and can be parsed with ease and convenience.

This tool is great if you want to monitor CPU and/or memory intensive cronjobs.

### Example
```
# ./proc -name php -grep "cronjob\.php listen_websockets" -stat
8650 8648 0% 39.7M 1649252760 2847 #php /var/www/php/cronjob.php listen_websockets
15272 15271 0.3% 38.9M 1649255581 26 #php /var/www/php/cronjob.php listen_websockets
```

## -name
The `-name <name>` filter all processes by name. If you want to list all PHP processes use `-name php`.

## -grep
The `-grep <pattern>` filter the result by a regex pattern before output in the same manner as piping `ps ... | grep ...`.

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

## Parse output with PHP
```
$procs = explode("\n", rtrim(shell_exec('./proc -name php -grep "cronjob\.php listen_websockets" -stat')));
foreach($procs as &$proc){
        $pos    = strpos($proc, '#');
        $cmd    = substr($proc, $pos+1);
        $values = explode(' ', rtrim(substr($proc, 0, $pos)));
        
        //  Output with -stat
        if($stat){
                $proc = [
                        'pid'   => '',
                        'ppid'  => '',
                        'cpu'   => '',
                        'mem'   => '',
                        'start' => '',
                        'time'  => '',
                        'cmd'   => $cmd
                ];
        }
        //  Output without -stat
        else{
                $proc = [
                        'pid'   => '',
                        'ppid'  => '',
                        'cmd'   => $cmd
                ];
        }
        
        foreach($proc as &$p){
                if($value = array_shift($values)){
                        $p = $value;
                }
        }
}

print_r($procs);
```