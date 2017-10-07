#!/usr/bin/env python3
# coding: utf8

from ctypes import *
import npyscreen
import sys

try:
    lib = cdll.LoadLibrary("./main.so")
except Exception as e:
    print("Could not load library:", e)


STDNTS_NDX_SLOTS = 64
STDNTS_NDX_SLOT = 64
STDNTS_MAX = STDNTS_NDX_SLOTS * STDNTS_NDX_SLOT

STDNT_NAME_LEN = 20
STDNT_STREET_LEN = 30


class AttributeDict(dict):
    __getattr__ = dict.__getitem__
    __setattr__ = dict.__setitem__


class housing_t:
    HOME = 0
    HOSTEL = 1

    values = [
        "home",
        "hostel"
    ]


class gender_t:
    MALE = 0
    FEMALE = 1

    values = [
        "male",
        "female"
    ]


class _home_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("house", c_uint8),
        ("room", c_uint8),
        ("street", c_char * (STDNT_STREET_LEN + 1)),
    ]


class _hostel_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("house", c_uint8),
        ("room", c_uint8),
    ]


class _address_u(Union):
    _pack_ = 1
    _fields_ = [
        ("home", _home_t),
        ("hostel", _hostel_t),
    ]


class student_t(Structure):
    _pack_ = 2
    _fields_ = [
        ("housing", c_uint32),
        ("gender", c_uint32),
        ("name", c_char * (STDNT_NAME_LEN + 1)),
        ("height", c_uint8),
        ("address", _address_u),
    ]


class students_index_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("slots", c_uint64 * STDNTS_NDX_SLOTS),
        ("ss", c_uint16 * STDNTS_MAX),
    ]


class students_t(Structure):
    _pack_ = 1
    _fields_ = [
        ("n", c_uint16),
        ("n_empty", c_uint16),
        ("data", student_t * STDNTS_MAX),
        ("ndx", students_index_t),
    ]


class AddressDatabase(object):
    def __init__(self):
        self.fn = sys.argv[1] if len(sys.argv) > 1 else "test.stud"
        self.ss = students_t()
        lib.init_students_t(byref(self.ss))
        lib.load_students(c_char_p(self.fn.encode("utf-8")),
                                byref(self.ss))

    def add_record(self, name, height):
        student = student_t()
        lib.init_student_t(byref(student))
        student.name = name.encode("utf-8")
        student.height = c_uint8(int(height))
        lib.clear_str(student.name, STDNT_NAME_LEN)
        lib.student_add(byref(self.ss), student)

    def update_record(self, record_id, name, height):
        self.delete_record(record_id)
        self.add_record(name, height)

    def delete_record(self, record_id):
        lib.student_del(byref(self.ss),
                        lib.get_student(record_id, byref(self.ss)).contents)

    def list_all_records(self, ):
        res = []

        lib.next_student.restype = c_uint16
        lib.get_student.restype = POINTER(student_t)

        i = lib.next_student(self.ss.n + self.ss.n_empty, byref(self.ss))
        while i != self.ss.n + self.ss.n_empty:
            p = i
            i = lib.next_student(
                    i,
                    byref(self.ss)
                )
            res.append(self._form_student(p))

        return res

    def get_record(self, record_id):
        return self._form_student(record_id)

    def _form_student(self, i):
        s = lib.get_student(i, byref(self.ss)).contents
        return AttributeDict(
            record_id=i,
            name=s.name.decode("utf-8"),
            height=s.height,
            gender=gender_t.values[s.gender],
            housing=housing_t.values[s.housing],
        )

    def save(self):
        lib.save_students(c_char_p(self.fn.encode("utf-8")),
                          byref(self.ss))

    def sort(self):
        lib.sort_students(byref(self.ss))


class RecordList(npyscreen.MultiLineAction):
    def __init__(self, *args, **keywords):
        super(RecordList, self).__init__(*args, **keywords)
        self.add_handlers({
            "^A": self.when_add_record,
            "^D": self.when_delete_record,
            "^S": self.save_database,
            "^R": self.sort,
        })

    def display_value(self, vl):
        return ("id{0.record_id:03d} {0.name:<30s} {0.housing:<10s} "
                "{0.height:>3d} "
                " {0.gender:>6s}").format(vl)

    def actionHighlighted(self, act_on_this, keypress):
        self.parent.parentApp.getForm('EDITRECORDFM').value = act_on_this.record_id
        self.parent.parentApp.switchForm('EDITRECORDFM')

    def when_add_record(self, *args, **keywords):
        self.parent.parentApp.getForm('EDITRECORDFM').value = None
        self.parent.parentApp.switchForm('EDITRECORDFM')

    def when_delete_record(self, *args, **keywords):
        self.parent.parentApp.myDatabase.delete_record(
            self.values[self.cursor_line].record_id)
        self.parent.update_list()

    def save_database(self, *args, **kwargs):
        self.parent.parentApp.myDatabase.save()

    def sort(self, *args, **kwargs):
        self.parent.parentApp.myDatabase.sort()
        self.parent.update_list()


class RecordListDisplay(npyscreen.FormMutt):
    MAIN_WIDGET_CLASS = RecordList
    def beforeEditing(self):
        self.update_list()

    def update_list(self):
        self.wMain.values = self.parentApp.myDatabase.list_all_records()
        self.wMain.display()


class EditRecord(npyscreen.ActionForm):
    def create(self):
        self.value = None
        self.s_name = self.add(npyscreen.TitleText, name="Name:",)
        self.height = self.add(npyscreen.TitleText, name="Height:")

    def beforeEditing(self):
        if self.value is not None:
            record = self.parentApp.myDatabase.get_record(self.value)
            self.name = "Record id : %s" % record.record_id
            self.record_id = record.record_id
            self.s_name.value = record.name
            self.height.value = str(record.height)
        else:
            self.name = "New Record"
            self.record_id = ''
            self.s_name.value = ''
            self.height.value = ''

    def on_ok(self):
        if self.record_id: # We are editing an existing record
            self.parentApp.myDatabase.update_record(
                self.record_id,
                name=self.s_name.value,
                height=self.height.value,
            )
        else: # We are adding a new record.
            self.parentApp.myDatabase.add_record(
                name=self.s_name.value,
                height=self.height.value,
            )
        self.parentApp.switchFormPrevious()

    def on_cancel(self):
        self.parentApp.switchFormPrevious()


class AddressBookApplication(npyscreen.NPSAppManaged):
    def onStart(self):
        self.myDatabase = AddressDatabase()
        self.addForm("MAIN", RecordListDisplay)
        self.addForm("EDITRECORDFM", EditRecord)

if __name__ == '__main__':
    try:
        myApp = AddressBookApplication()
        myApp.run()
    except KeyboardInterrupt:
        pass
