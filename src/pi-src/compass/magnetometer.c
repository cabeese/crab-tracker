 #define _GNU_SOURCE    

#include <stdio.h>
    #include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h> 
    #include <fcntl.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/ioctl.h>
    #include <sys/types.h>
    #include <sys/stat.h>
#include <sys/time.h>
    #include <linux/i2c-dev.h>

    const int HMC5883L_I2C_ADDR = 0x1E;

#define CONFIG_A 0x00
#define CONFIG_B 0x01
#define MODE 0x02
#define DATA 0x03 //read 6 bytes: x msb, x lsb, z msb, z lsb, y msb, y lsb
#define STATUS 0x09
#define ID_A 0x0A
#define ID_B 0x0B
#define ID_C 0x0C
#define ID_STRING "H43"

#define GAIN 1370 //000 setting

    void selectDevice(int fd, int addr, char * name)
    {
        if (ioctl(fd, I2C_SLAVE, addr) < 0)
        {
            fprintf(stderr, "%s not present\n", name);
            //exit(1);
        }
    }

    void writeToDevice(int fd, int reg, int val)
    {
        char buf[2];
        buf[0]=reg;
        buf[1]=val;

        if (write(fd, buf, 2) != 2)
        {
            fprintf(stderr, "Can't write to ADXL345\n");
            //exit(1);
        }
    }

   

    int main(int argc, char **argv)
    {
        int fd;
        unsigned char buf[16];
	struct timeval tv;
	struct timezone tz; //ignored
	struct timeval data_timestamp; //
	int resolution = 100000; //microseconds
	long next_timestamp; 

        if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)
        {
            // Open port for reading and writing
            fprintf(stderr, "Failed to open i2c bus\n");

            return 1;
        }

        /* initialise HMC5883L */

        selectDevice(fd, HMC5883L_I2C_ADDR, "HMC5883L");

	//first read the 3 ID bytes
	buf[0] = ID_A;
	if ((write(fd, buf, 1)) != 1)
            {
                // Send the register to read from
                fprintf(stderr, "Error writing to i2c slave\n");
            }

            if (read(fd, buf, 3) != 3) {
                fprintf(stderr, "Unable to read from HMC5883L\n");
            }
	buf[3]=0;
	printf("Identification: '%s' ",buf);
	if (strncmp(buf, ID_STRING, 3) == 0) 
		printf("HMC5883L sensor detected\n");
	else {
		printf("unknown sensor. Exiting.\n");
		exit(1);
		}

	//Configuration
	
        //writeToDevice(fd, 0x01, 0);
	writeToDevice(fd, CONFIG_A, 0b01101000); //8 sample averaging
        writeToDevice(fd, CONFIG_B, 0b00000000); //max gain
        writeToDevice(fd, MODE,     0b00000011); //idle mode

	//find current time
	gettimeofday(&data_timestamp,&tz);
	data_timestamp.tv_sec += 1; //start loggin at start of next second
	data_timestamp.tv_usec = 0;
           
        while(1){ //record forever   

		//read time & wait until next reading required
		
		while (1) {
			//get time
			gettimeofday(&tv,&tz);

			//if seconds >= next_secs && usecs >= next_usecs
			if (tv.tv_sec >= data_timestamp.tv_sec && tv.tv_usec >= data_timestamp.tv_usec) 
				break;

			usleep(1000);
		}

		//initiate single conversion
		writeToDevice(fd, MODE,     0b00000001); //single measurement

		//wait 7 milliseconds
		usleep(7000);

            buf[0] = DATA;

            if ((write(fd, buf, 1)) != 1)
            {
                // Send the register to read from
                fprintf(stderr, "Error writing to i2c slave\n");
            }

            if (read(fd, buf, 6) != 6) {
                fprintf(stderr, "Unable to read from HMC5883L\n");
            } else {


                short x = (buf[0] << 8) | buf[1];
                short y = (buf[4] << 8) | buf[5];
                short z = (buf[2] << 8) | buf[3];
               
                //float angle = atan2(y, x) * (180.0 / M_PI);
                //I don't know how or why this math works. But it makes the digital compass agree with my phone's compass. That is disconcerting. 
                
                //float angle = atan2(y, x) * (180.0 / M_PI)-90;
                //if (angle > 0) {angle -= 360;}
                //angle += 360;

		            //float mag = sqrt(x*x+y*y+z*z);
                
                float angle =  atan2(y, x) * (180.0 / M_PI);
                
                float magnetic_heading = atan2(y, x);
               
                //printf("x=%d, y=%d, z=%d\n", x, y, z);
                printf("angle = %0.1f\n", angle);
                //printf("angle = %0.1f, mag = %0.1f\n", angle,mag);
                //printf("magnetic heading %0.2f \n", magnetic_heading);
		//printf("time: %ld.%06ld\n",tv.tv_sec, tv.tv_usec);
    // Orignal print statement:            
		//printf("%ld.%06ld, %d, %d, %d, %0.1f nT\n",data_timestamp.tv_sec, data_timestamp.tv_usec,x,y,z,1e5*mag/GAIN);

    //printf("%0.1f nT\n", 1e5*mag/GAIN); // this will print out the magnetic field strength
		fflush(stdout);
            }

	//advance data timestamp to next required time
	data_timestamp.tv_usec += resolution;
	if (data_timestamp.tv_usec >= 1e6) {
		data_timestamp.tv_sec += 1;
		data_timestamp.tv_usec -= 1e6;
		}
           
        }

        return 0;
    }

