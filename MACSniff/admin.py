from django.contrib import admin
from .models import Router,ClassRoom,Student

class RouterAdmin(admin.ModelAdmin):
	list_display = ('router','mac')
	list_display_links = ('router',)

admin.site.register(Router,RouterAdmin)

class ClassRoomAdmin(admin.ModelAdmin):
	list_display = ('name','router')
	list_display_links = ('name',)

admin.site.register(ClassRoom,ClassRoomAdmin)

class StudentAdmin(admin.ModelAdmin):
	list_display = ('name','roll_number','mac')
	list_display_links = ('name',)

admin.site.register(Student,StudentAdmin)