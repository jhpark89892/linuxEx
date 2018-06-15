#include <linux/fs.h>		//open(),read(),write(),close()
#include <linux/cdev.h>		//register_chardev_region(),cdev_init()
#include <linux/module.h>
#include <linux/io.h>		//ioremap(),iounmap()
#include <linux/uaccess.h>	//copy_from_user(),copy_to_user()
#include <linux/gpio.h>		//request_gpio(), gpio_set_val(), gpio_get_val()
#include <linux/interrupt.h>	//gpio_to_irp(), request_irp()
#include <linux/timer.h>	//init_timer(),mod_timer(),del_timer(),del_timer_sync
//#include <asm/siginfo.h>	//siginfo 구조체를 사용하기 위해
#include <linux/sched/signal.h>
#include <linux/sched.h>

#define GPIO_MAJOR 200
#define GPIO_MINOR 0
#define GPIO_DEVICE "gpioled"

//Raspi 0,1 PHYSICAL I/O PERI BASE ADDR
//#define BCM_IO_BASE 0x20000000

// Raspi 2,3 PHYSICAL I/O PERI BASE ADDR
#define BCM_IO_BASE 0x3F000000
#define GPIO_BASE (BCM_IO_BASE + 0x200000)

#define GPIO_LED 27
#define GPIO_LED2 26
#define GPIO_SW 17
#define GPIO_SW2 16
#define BUF_SIZE 100

static char msg[BUF_SIZE]={0};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HJ Park");
MODULE_DESCRIPTION("Raspberry Pi First Device Driver");

struct cdev gpio_cdev;
static int switch_irq;
static int switch_irq2;
static struct timer_list timer; //타이머를 위한 구조체
static struct task_struct *task; //task를 위한 구조체

pid_t pid;
char pid_valid;

static int gpio_open(struct inode *inod, struct file *fil);
static int gpio_close(struct inode *inod, struct file *fil);
static ssize_t gpio_write(struct file *inode, const char *buff, size_t len, loff_t * off);
static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off);

static struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
        .read  = gpio_read,
        .write = gpio_write,
	.open = gpio_open,
	.release = gpio_close,
};

static void timer_func(unsigned long data)
{
   printk(KERN_INFO "timer_func : %ld\n",data);
   gpio_set_value(GPIO_LED,data);
   if(data)
    	timer.data=0;
    else
	timer.data=1;
    timer.expires=jiffies+msecs_to_jiffies(500);
    add_timer(&timer);
}

static irqreturn_t isr_func (int irq, void *data)
{
    static int count;
    static struct siginfo sinfo;
    
    if((irq=switch_irq) && !(gpio_get_value(GPIO_LED)))
    {
	gpio_set_value(GPIO_LED, 1);
	
	memset(&sinfo, 0, sizeof(struct siginfo));
	sinfo.si_signo = SIGIO;
	sinfo.si_code = SI_USER;

	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task!=NULL)
	{
	    	strcpy(msg, "switch1 pushed");
		send_sig_info(SIGIO, &sinfo, task);
	}
	else
	{
		printk("Error : I don't know user pid\n");
	}
	
    }
    else
	gpio_set_value(GPIO_LED, 0);
    printk(KERN_INFO "Called isr_func() : %d\n", count);
    count++;
    return IRQ_HANDLED;
}

static irqreturn_t isr_func2 (int irq, void *data)
{
    static int count;
    static struct siginfo sinfo;

    if((irq=switch_irq) && !(gpio_get_value(GPIO_LED2)))
    {
	gpio_set_value(GPIO_LED2, 1);

	memset(&sinfo, 0, sizeof(struct siginfo));
	sinfo.si_signo = SIGIO;
	sinfo.si_code = SI_USER;

	task = pid_task(find_vpid(pid), PIDTYPE_PID);
	if(task!=NULL)
	{
	    strcpy(msg, "switch2 pushed");
	    send_sig_info(SIGIO, &sinfo, task);
	}
	else
	{
	    printk("Error : I don't know user pid\n");
	}

    }
    else
	gpio_set_value(GPIO_LED2, 0);
    printk(KERN_INFO "Called isr_func() : %d\n", count);
    count++;
    return IRQ_HANDLED;
}


static int gpio_open(struct inode *inod, struct file *fil)
{
	try_module_get(THIS_MODULE);
	printk(KERN_INFO "GPIO Device opened()\n");
	return 0;
}

static int gpio_close(struct inode *inod, struct file *fil)
{
	module_put(THIS_MODULE);
	printk(KERN_INFO " GPIO Device closed()\n");
	return 0;
}

static ssize_t gpio_write(struct file *inode, const char *buff, size_t len, loff_t * off)
{
    short count;
    char *cmd, *str;
    char *sep = ":";
    char *pidstr;
    int sel;
    memset(msg, 0, BUF_SIZE);
    count = copy_from_user(msg, buff, len);
    str = kstrdup(msg, GFP_KERNEL);
    cmd = strsep(&str,sep);
    pidstr = strsep(&str,sep);
    cmd[1] = '\0';
    printk("Command : %s, PID : %s\n", cmd, pidstr);
   
    /*
    if((!strcmp(cmd, "0")))
    {
	del_timer_sync(&timer);
    }
    else
    {
	init_timer(&timer);
	timer.function=timer_func;	//expire 호출하는 함수
	timer.data =1;			//timer_func으로 전달하는 인자값
	timer.expires = jiffies+msecs_to_jiffies(500);
	add_timer(&timer);
    }
    */
    
    sel = (int)(cmd[0]-'0');
    printk(KERN_INFO "sel : %d\n", sel);

    switch(sel)
    {
	case 1:
	    gpio_set_value(GPIO_LED, 1);
	    gpio_set_value(GPIO_LED2,0);
	    break;
	case 2:
	    gpio_set_value(GPIO_LED, 1);
	    gpio_set_value(GPIO_LED2, 1);
	    break;
	case 3:
	    gpio_set_value(GPIO_LED, 0); 
	    gpio_set_value(GPIO_LED2,1);
	    break;
	default:
	    gpio_set_value(GPIO_LED, 0);
	    gpio_set_value(GPIO_LED2,0);
	    break;
    }

    printk(KERN_INFO "GPIO Device Write : %s\n", msg);
    pid = simple_strtoul(pidstr,NULL, 10);
    printk(KERN_INFO "PID = %d\n", pid);
    return count;
}

static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off)
{
    int count;
    strcat(msg, " from kernel");
    
   /*
    if(gpio_get_value(GPIO_LED))
	msg[0]='1';
    else
	msg[0]='0';
   */
    //strcat(msg,"from kernel");

    //커널의 msg문자열을 사용자영역(buff의번지)으로 복사한다.
    count = copy_to_user(buff, msg, strlen(msg)+1);
    printk(KERN_INFO "GPIO Device read:%s\n", msg);
    return count;
}

static int __init initModule(void)
{
	dev_t devno;
	unsigned int count;

	int err;
	// 0. 함수 호출 유무를 확인하기 위해
	printk(KERN_INFO "Init gpio_module\n");

	// 1. 문자 디바이스 번호와 이름을 등록
	devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	printk(KERN_INFO "devno=0x%x\n",devno);
	register_chrdev_region(devno,1,GPIO_DEVICE);

	// 2. 문자 디바이스를 위한 구조체를 초기화 한다.
	cdev_init(&gpio_cdev, &gpio_fops);
	count = 1;

	// 3. 문자 디바이스 추가
	err = cdev_add(&gpio_cdev, devno, count);
	if(err<0)
	{
	    printk(KERN_INFO "Error : cdev_add()\n");
	    return -1;
	}

	//application에서 open함수 if-else문에 넣는것이 좋음
	printk(KERN_INFO "'mknod /dev/%s c %d 0'\n", GPIO_DEVICE, GPIO_MAJOR);
	printk(KERN_INFO "'chmod 666 /dev/%s'\n", GPIO_DEVICE);

	//gpio.h에 정의된 gpio_request함수사용
	err = gpio_request(GPIO_LED, "LED");
	if(err == -EBUSY)
	{
	    printk(KERN_INFO "Error gpio_request : in used\n");
	    return -1;
	}

	err = gpio_request(GPIO_LED2, "LED2");
	if(err == -EBUSY)
	{
	    printk(KERN_INFO "Error gpio_request : in used\n");
	    return -1; 
	}

	err=gpio_request(GPIO_SW, "SW");
	if(err == -EBUSY)
	{
	    printk(KERN_INFO "Error gpio_request : in used\n");
	    return -1;
	}
	switch_irq = gpio_to_irq(GPIO_SW);
	request_irq(switch_irq, isr_func, IRQF_TRIGGER_RISING, "switch", NULL);
	
	err=gpio_request(GPIO_SW2, "SW2");
	if(err == -EBUSY)
	{
	    printk(KERN_INFO "Error gpio_request : inused\n");
	}
	switch_irq2 = gpio_to_irq(GPIO_SW2);
	request_irq(switch_irq2, isr_func2, IRQF_TRIGGER_RISING, "switch2", NULL);

	gpio_direction_output(GPIO_LED, 0);
	gpio_direction_output(GPIO_LED2, 0);
	return 0;
}

static void __exit cleanupModule(void)
{
        dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
	del_timer_sync(&timer);
        // 1. 문자 디바이스의 등록(장치번호, 장치명)을 해제한다.
        unregister_chrdev_region(devno, 1);

	// 2. 문자 디바이스의 구조체를 제거한다.
	cdev_del(&gpio_cdev);

	free_irq(switch_irq, NULL);
	free_irq(switch_irq2,NULL);
	gpio_direction_output(GPIO_LED,0);
	gpio_free(GPIO_LED);
	gpio_direction_output(GPIO_LED2,0);
	gpio_free(GPIO_LED2);
	gpio_free(GPIO_SW);
	gpio_free(GPIO_SW2);

        printk(KERN_INFO "Exit gpio_module : Good-bye\n");
}

//내가 생성하고자 하는 초기화함수 이름을 적어준다.
module_init(initModule);
//내가 생성하고자 하는 종료함수 이름을 적어준다.
module_exit(cleanupModule);

