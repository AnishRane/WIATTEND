from django.db import models

class Router(models.Model):
	router = models.CharField(max_length = 255,unique = True)
	mac = models.CharField(max_length = 17,unique = True)

	def __str__(self):
		return self.router

class ClassRoom(models.Model):
	name = models.CharField(max_length=255)
	router = models.ForeignKey(Router,on_delete = models.CASCADE)

	def __str__(self):
		return self.name

class Student(models.Model):
	name = models.CharField(max_length=255)
	roll_number = models.CharField(max_length=10,unique=True)
	mac = models.CharField(max_length=17,unique=True)

	def __str__(self):
		return self.name

class Attendance(models.Model):
	student = models.ForeignKey(Student,on_delete = models.CASCADE)
	class_room = models.ForeignKey(ClassRoom,on_delete = models.CASCADE)