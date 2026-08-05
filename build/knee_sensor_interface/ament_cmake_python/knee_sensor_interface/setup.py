from setuptools import find_packages
from setuptools import setup

setup(
    name='knee_sensor_interface',
    version='0.0.0',
    packages=find_packages(
        include=('knee_sensor_interface', 'knee_sensor_interface.*')),
)
