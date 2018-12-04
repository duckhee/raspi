#include <linux/fs.h>          //open(), read(), write(), clse() kernel function
#include <linux/cdev.h>        //char device
#include <linux/module.h>
#include <linux/io.h>          //ioremap(), ioumap(), kernel function
#include <linux/gpio.h> 
#include <asm/uaccess.h>       //copy_to_user()m copy_from_user() kernel function
#include <mach/platform.h>     //gpio_base macro

MODULE_LICENSE("GPL");
MODULE_AUTHOR("duckheewon");
MODULE_DESCRIPTION("Raspberry Pi GPIO LED device Module");

//device first number and sub number
#define GPIO_MAJOR       200
#define GPIO_MINOR       0
#define GPIO_DEVICE      "gpioled"         //device product file name
#define GPIO_LED         18               //use LED for GPIO number

static char msg[BLOCK_SIZE] = {0};       //write() function read data save

//io function set
static int gpio_open(struct inode *, struct file *);
static ssize_t gpio_read(struct file *, char size_t loff_t *);
static ssize_t gpio_write(struct inode *, const char *, size_t loff_t *);
static int gpio_close(struct inode *, struct file *);

//unix io function deal struct
static struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .read = gpio_read,
    .write = gpio_write,
    .open = gpio_open,
    .release = gpio_close,
};

struct cdev gpio_cdev;

static int gpio_open(struct indoe *inod, struct file *fil)
{
    printk("GPIO Device opened(%d/%d)\n", imajor(inod)), iminor(inod);

    return 0;
}

static int gpio_close(struct inode *inod, struct file *fil)
{
    printk("Gpio Device closed(%d)\n", MAJOR(fil->f_dentry->d_inode->i_rev));

    return 0;
}

static ssize_t gpio_read(struct file *inode, char *buff, size_t len, loff_t *off)
{
    int count;

    strcat(msg, "from kernel");
    count = copy_to_user(buff, msg, strlen(msg)+1);     //send user mode

    printk("GPIO Device (%d) read : %s(%d)\n", MAJOR(inode->f_dentry->d_inode->i_rdev), msg, count);

    return count;
}

static ssize_t gpio_write(struct file *inode, const char *buff, size_t len, loff_t *off)
{
    short count;
    memset(msg, 0, BLOCK_SIZE);
    count = copy_from_user(msg, buff, len);      //get user mode

    //set led
    gpio_set_value(GPIO_LED, (!strcmp(msg, "0"))?0:1);

    printk("GPIO Device(%d) write : %s(%d)\n", MAJOR(indoe->f_dentry->d_inode->i_rdev), msg, len);

    return count;
}

int initModule(void)
{
    dev_t devno;
    unsigned int count;
    //static void *map;          //io connect variable
    int err;

    printk(KERN_INFO "Hello module!\n");

    try_module_get(THIS_MODULE);

    //insert char device
    devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
    register_chrev_region(devno, 1, GPIO_DEVICE);

    //char device for struct set
    cdev_init(&gpio_cdev, &gpio_fops);

    gpio_cdev.owner = THIS_MODULE;
    count = 1;
    err = cdev_add(&gpio_cdev, devno, count);  //add char device
    if(err < 0)
    {
        printk("Error : device add\n");
        return -1;
    }

    printk("mknod /dev/%s c %d 0\n", GPIO_DEVICE, GPIO_MAJOR);
    printk("chmode 666 /dev/%s\n", GPIO_DEVICE);

    //gpio use call
    gpio_request(GPIO_LED, "LED");
    gpio_direction_output(GPIO_LED, 0);

    return 0;
}

void cleanupModule(void)
{
    dev_t devno = MKDEV(GPIO_MAJOR, GPIO_MINOR);
    unregister_chrdev_region(devno, 1);   //disconnect char device

    cdev_del(&gpio_cdev);    //dis link char device struct

    //not more need releasd
    gpio_free(GPIO_LED);

    module_put(THIS_MODULE);

    printk(KERN_INFO "Good-Bye module!\n");
}

module_init(initModule);
module_exit(cleanupModule);