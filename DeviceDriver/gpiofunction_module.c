#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

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
#define BUF_SIZE 100

static char msg[BUF_SIZE]={0};

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HJ Park");
MODULE_DESCRIPTION("Raspberry Pi First Device Driver");

struct cdev gpio_cdev;
static int switch_irq;

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

static irqreturn_t isr_func (int irq, void *data)
{
    static int count;
    if(irq=switch_irq && !(gpio_get_value(GPIO_LED)))
	gpio_set_value(GPIO_LED, 1);
    else
	gpio_set_value(GPIO_LED, 0);
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
    int sel;
    memset(msg, 0, BUF_SIZE);
    count = copy_from_user(msg, buff, len);
   
    sel = (int)(msg[0]-'0');
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
    return count;
}

static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off)
{
    int count;
    strcat(msg, " from kernel");
    
    if(gpio_get_value(GPIO_LED))
	msg[0]='1';
    else
	msg[0]='0';

    strcat(msg,"from kernel");

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

	gpio_direction_output(GPIO_LED, 0);
	gpio_direction_output(GPIO_LED2, 0);
	return 0;
}

static void __exit cleanupModule(void)
{
        dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
        // 1. 문자 디바이스의 등록(장치번호, 장치명)을 해제한다.
        unregister_chrdev_region(devno, 1);

	// 2. 문자 디바이스의 구조체를 제거한다.
	cdev_del(&gpio_cdev);

	free_irq(switch_irq, NULL);
	gpio_direction_output(GPIO_LED,0);
	gpio_free(GPIO_LED);
	gpio_direction_output(GPIO_LED2,0);
	gpio_free(GPIO_LED2);
	gpio_free(GPIO_SW);

        printk(KERN_INFO "Exit gpio_module : Good-bye\n");
}

//내가 생성하고자 하는 초기화함수 이름을 적어준다.
module_init(initModule);
//내가 생성하고자 하는 종료함수 이름을 적어준다.
module_exit(cleanupModule);

