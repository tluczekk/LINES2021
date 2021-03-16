import smtplib
import ssl
import datetime

# Inspired by:
# https://realpython.com/python-send-email/

# MiNI credentials
out_port = 465
email_send = input("From:\t")
email_rec = input("To:\t")
login_mini = input("Login:\t")
password_mini = input("Pass:\t")

# message
message = """\
Subject: Buildroot system is up

Your Buildroot system built for LINES course has been turned on at {0}.""".format(datetime.datetime.now())

# SSL context
context = ssl.create_default_context()

with smtplib.SMTP_SSL("poczta.mini.pw.edu.pl", out_port, context=context) as server:
    server.login(login_mini, password_mini)
    server.sendmail(email_send, email_rec, message)
