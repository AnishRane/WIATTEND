from django.shortcuts import render,redirect
from .models import Router, ClassRoom, Attendance, Student
from rest_framework.decorators import api_view
from django.views.decorators.csrf import csrf_exempt
from rest_framework.response import Response


def index(request):

	classrooms = ClassRoom.objects.all()

	context_data = {
		'classrooms' : classrooms,
	}

	return render(request,'index.html',context_data);

def create_classroom(request):
	if(request.method == 'POST'):
		name = request.POST.get('class_name')
		router = Router.objects.get(pk = request.POST.get('router'))

		class_room = ClassRoom(name = name,router = router)
		class_room.save()

		return redirect('index')

	routers = Router.objects.all()

	context_data = {
		'routers' : routers
	}

	return render(request,'create_classroom.html',context_data)

def view_classroom(request,c_id):
	class_room = ClassRoom.objects.get(pk=c_id)

	attendance = Attendance.objects.filter(class_room  = class_room)

	context_data = {
		'classroom' : class_room,
		'attendance' : attendance
	}


	return render(request,'view_classroom.html',context_data)

@api_view(['GET'])
def api_set_attendance(request):
	print(request.POST)
	student = Student.objects.get(mac = request.GET.get('device'))
	router = Router.objects.get(mac = request.GET.get('ap'))
	classroom = ClassRoom.objects.get(router = router)

	try: 
		Attendance.objects.get_or_create(student = student,class_room = classroom)

		response_json = {
			'status' : 'Success'
		}
	except Exception as e:
		print(e)
		response_json = {
			'status' : 'Failed'
		}


	return Response(response_json)