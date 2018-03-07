# ECS 160 W18 Project 4

## Contributors

- [Dutta, Rishi](https://github.com/RishuvDutta)
- [Ngo, Bryan](http://bryngo.me)


## Notable Problems
- Remembering how to write in C


## Notable things to check for valid input
- Number of columns is consistent line to line
    - Possibly check if each of the expected header column names are found (i.e. `name`, `user_timezone`, etc)
- No extra command in headers / any line
    - Helpful note: no commas should be found within the actual tweet, or any other column
- Check that the actual file format is csv
    - Also check what happens on empty file input