# cgroups-notes

## Links
[man page](https://man7.org/linux/man-pages/man7/cgroups.7.html)

## Brief
Cgroups are controlled via virtual file system on /sys/fs/cgroup.  

## cgroup v1 example
The following commands are limiting CPU share for process with PID of 21764:
``` bash
cd /sys/fs/cgroup

# Create directory for CPU controller (CPU shares limit)
sudo mkdir cpu

sudo mount -t cgroup -o cpu none /sys/fs/cgroup/cpu
sudo mkdir /sys/fs/cgroup/cpu/my_cgroup
```

``` bash
cd my_cgroup

# Add PID to cgroup's tasks
echo "21764" | sudo tee tasks

# Set the CPU shares for the cgroup
echo "16" | sudo tee cpu.shares
```

If you use the `test_app` that has infinite loop as example app here,  
note that it will still have 100% cpu consumption.   
If you want to see the cgroup's CPU limit effect you have to run  
multiple instances of `test_app` while only one is in cgroup.  
The instance that is in cgroup with limited CPU will consume less  
CPU than the rest.   

cgroup directory content example:
``` bash
pi@raspberrypi:/sys/fs/cgroup/cpu/my_cgroup $ ls -lrt
total 0
-rw-r--r-- 1 root root 0 Oct 15 01:24 notify_on_release
-r--r--r-- 1 root root 0 Oct 15 01:24 cpu.stat
-rw-r--r-- 1 root root 0 Oct 15 01:24 cpu.idle
-rw-r--r-- 1 root root 0 Oct 15 01:24 cpu.cfs_quota_us
-rw-r--r-- 1 root root 0 Oct 15 01:24 cpu.cfs_period_us
-rw-r--r-- 1 root root 0 Oct 15 01:24 cpu.cfs_burst_us
-rw-r--r-- 1 root root 0 Oct 15 01:24 cgroup.procs
-rw-r--r-- 1 root root 0 Oct 15 01:24 cgroup.clone_children
-rw-r--r-- 1 root root 0 Oct 15 02:12 tasks
-rw-r--r-- 1 root root 0 Oct 15 02:15 cpu.shares

```

## Checking cgroups version and supported controllers
You can check this by looking at the /proc/cgroups file.  
If it contains 1 next to the cgroup2 entry, it means Cgroups v2 is supported.  
Each controller (resource like cpu,mem,etc.) has `enabled` flag.

``` bash
pi@raspberrypi:/sys/fs/cgroup $ cat /proc/cgroups
#subsys_name    hierarchy       num_cgroups     enabled
cpuset  0       59      1
cpu     0       59      1
cpuacct 0       59      1
blkio   0       59      1
memory  0       59      0
devices 0       59      1
freezer 0       59      1
net_cls 0       59      1
perf_event      0       59      1
net_prio        0       59      1
pids    0       59      1
```

## cgroup file system

cgroups kernel code is located under `linux/kernel/cgroup` directory.  
`cgroup.c` has call for `kernfs_create_root` kernel API to create new  
file system.  

Above we used the command `sudo mount -t cgroup -o cpu none /sys/fs/cgroup/cpu`  
in order to add new controller to the cgroups.  
This is implemented via the cgroup file system.  
The read/write and other operations are described in `cgroup.c` file.  
[cgroup.c link](https://elixir.bootlin.com/linux/v6.1.58/source/kernel/cgroup/cgroup.c#L2086)  
  