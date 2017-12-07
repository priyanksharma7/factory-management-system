#This is a sample bash script for q2.
#You can modify it for your benchmark experiments in q3


num_space=80
num_cars=4
num_workers=8
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=14
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=16
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=25
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=32
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=48
./tesla_factory.out $num_cars $num_space $num_workers

num_workers=64
./tesla_factory.out $num_cars $num_space $num_workers
