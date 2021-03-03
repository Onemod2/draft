names = ["Bob", "Marina", "Ivan", "Roman", "Nastya"]
second_names = ["Frolov", "Ivanova", "Semianov", "Shirayev", "Krilova"]
average_marks = [4.5, 4.0, 3.0, 5.0, 4.3]
groups = [101, 21, 232, 485, 1]

students = []

# range(start=0, stop, step=1); range(0, 200, 5) = [0, 5, 10, 15, 20 ... 195]
# len() - length() длинна
# len(names) = 5
# range(5) = [0, 1, 2, 3, 4]


for i in range(len(names)):
    student = {"name": names[i], "second_name": second_names[i], "average_mark": average_marks[i], "group": groups[i]}
    students.append(student)

# sum of average_marks
s = 0

for student in students:
    s = s + student["average_mark"]

print(s / len(students))
