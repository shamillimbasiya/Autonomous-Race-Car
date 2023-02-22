from rplidar import RPLidar

PORT_NAME = '/dev/ttyUSB0'
BAUDRATE = 115200
lidar = RPLidar(PORT_NAME, BAUDRATE, timeout=3)
lidar.stop()
lidar.stop_motor()
