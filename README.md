# Philosophers

Mandatory part without any bonus

Success 100/100

## Installation

```bash
  git clone https://github.com/sleepychloe/Philosopers.git
  cd philo
  make
```

## Usage

```bash
  ./philo [num_of_philo] [time_to_die] [time_to_eat] [time_to_sleep]
  ./philo [num_of_philo] [time_to_die] [time_to_eat] [time_to_sleep] [num_of_time_each_philo_must_eat]
```

example
```bash
./philo 1 800 200 200
./philo 5 800 200 200
./philo 5 800 200 200 7
./philo 4 410 200 200
./philo 4 310 200 100
```

to check data race and potential deadlock, use -fsanitize=thread
