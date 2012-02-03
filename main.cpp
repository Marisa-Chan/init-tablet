#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define USB_VENDOR_ID_KYE       0x0458
#define USB_PRODUCT_ID_i608x    0x5011

int main()
{

	libusb_context *ctx = NULL;

	int r = libusb_init(&ctx);

	if(r < 0)
	{
		printf("Init Error\n");
        return 1;
	}

	libusb_set_debug(ctx, 3);

    libusb_device_handle* dvs = libusb_open_device_with_vid_pid(ctx,USB_VENDOR_ID_KYE,USB_PRODUCT_ID_i608x);

    if (dvs)
    {
        int32_t TO_WRITE = 8;
        unsigned char data[8]={0x5,0x12,0x10,0x11,0x12,0,0,0};
        //unsigned char data2[8]={0x2,0x10,0x02,0x00,0x00,0,0,0};
        //unsigned char data2[8]={0x2,0x10,0x01,0x00,0x00,0,0,0};

        libusb_detach_kernel_driver(dvs,0);

        int res = libusb_control_transfer(dvs,LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                                              LIBUSB_REQUEST_SET_CONFIGURATION,0x305,0,data,TO_WRITE,0);

        //  int res = libusb_control_transfer(dvs,LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
        //                                     LIBUSB_REQUEST_SET_CONFIGURATION,0x302,0,data2,TO_WRITE,0);

        if (res == TO_WRITE)
            printf("OK\n");
        else
            printf("Err: %d\n",res);

        libusb_attach_kernel_driver(dvs,0);
        libusb_reset_device(dvs);
        libusb_close(dvs);
    }

    libusb_exit(ctx);
    return 0;
}
