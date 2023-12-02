
import firebase_admin
from firebase_admin import db, credentials

#Global

ref_global = 0

credentials_database = {
    "type": "service_account",
    "project_id": "cms-rasp",
    "private_key_id": "4a588f4ce15b0f5cc02606f77bc653a1318b253c",
    "private_key": "-----BEGIN PRIVATE KEY-----\nMIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC/09gRo9jOleJ/\nEpIcZKiPdYOo+WL8BsVbRW69d5QKX7Lw/V/NimtWfjmXyE7+3ZwubMwjfpvxMR71\n0EGiOFtSd14QAZfuJpxOFP9zSxMmxhTFgwtSeZX/bc1pBYOb4nZx6qSurnLAV05D\nHQFj39hvw36DunjoGviFx6My4vXfh8PGSE60OK0VYL1nRD9M93gtTJbv8Q5pr5hh\nfzTBRdi5SEMJueVHyHt/Gzx3RupGO9AybZxqMSfYAy5me2p9lLVru3IgdPzmYfS5\nlyOCmRU7rD3pnSKxlQxH6AFMIwxGVYfh7J3SmTFGeyJj7qNwdK7wuJydCl7vYM4w\nNqeOrNjZAgMBAAECggEAB/Yi4ZiLRwexftSme8cxt8KI1FqFYTBmwIMBNZvg6Q1P\nv/DEKSO1kMFsYoBNxwYrlUGut0pp7ni7cDhUgQcVwzZxEPj/9UJk+likwYQ8Jz/R\nXOEzdxjUwOUtwU4ZSD7963xxmU5kNNdqyB5MEHkZ4omalp0Gp5t29RZ0D4ib68sE\nRLepXOpYUcto+YjGNiadzIkSVnTX1UjPRJFBW1+Y7veEjTizVxIwSgbWaNRV9Zo1\nI+ffM/Zje48u84n2Ai3u6p/JNMG6jfdvX2tQK3xTpetyyrFwFLwqGZUIm12NdinO\n0xaSK3V8ixQzLu9Je90vQa6eWRkiKCt/YOz7yYwAAQKBgQDtgJB2+LJGjG++d4iH\nEiM5EttMHBzGgR6zIZ9jfBLPnKDfGUV7DiwG/Hf9zT6+niR7Ld03lSnxbmfQlYNz\nvcQtUU6o75MMTrMsNtvzfKiXmHJqpiOBENY1iNJzb3mRekeoUXPntdHqxmoiObmr\nXkkUtRSIPymxOVAqFB5wHf37AQKBgQDOxJhmXE4wr75Jw+XohOp2hjgswRUx33hf\neQxvtIt6MDAYMvjhNhQ7pJorHnq7STRwTSCqfqqnK51jlWaAJdf1OiusJtFwnwZf\n9a2sXx2/r1ukANF24doN7FebYJmXDyJ8d96d08aFsbDJqHfw/3jcmTSkNIPCq76c\n6r4sSMwV2QKBgBDsfl7b7eg1Dalm3yWw1y2z+Eh+IFbM0KE3HiJ6ruR4ZsY/CMJy\nChUdXHeLbpMqtbLJW+AgEvMPSPqGDO3scdRA3PgQykuwN/hImHfLREA5Lr4GxLTn\nvDvoWga7xM/nBMArvbSfMiwmo114+5AIkfCVzra+uXvVd67n7BtVJ8oBAoGAWlIe\nkHg536KlaOud/pgxI102JLajX4nSt0HVYOZpFxL9r1R0y0woQnWAst05l54+vd+z\nKUbznNq8gce9YrUlRQYIFcdPLfcJfdl/6BAUmqsrSNTiqx+1dOeMzV7BOO9vjRd/\ns0nTKouYQj7JRgkS7qouCJy4Ac2sEjeooweZvuECgYEApZjYgbEF4/JHcbRcUL+L\nNEci+cppvANJlnvX3rsHJCigwX5zWY8Y7yOgn4e1IAlQzEMby6CXAfYTEQszseNV\ntes8ZzOgm57hU/z3gxj1GqlizqaaooaIOyZW2lJNyZk9X3Kk3oTLL6E7EfjQNIiL\nOF/8e7dIsW5aLIe4jPca7oM=\n-----END PRIVATE KEY-----\n",
    "client_email": "firebase-adminsdk-sjnbn@cms-rasp.iam.gserviceaccount.com",
    "client_id": "103748681409716167236",
    "auth_uri": "https://accounts.google.com/o/oauth2/auth",
    "token_uri": "https://oauth2.googleapis.com/token",
    "auth_provider_x509_cert_url": "https://www.googleapis.com/oauth2/v1/certs",
    "client_x509_cert_url": "https://www.googleapis.com/robot/v1/metadata/x509/firebase-adminsdk-sjnbn%40cms-rasp.iam.gserviceaccount.com",
    "universe_domain": "googleapis.com"
  }

#EXTRA FUNCTIONS

def dict_sensors(name,type,room):

    return {
        "Name": name,
        "Room": room,
        "Type": type
    }

#----------------------------------

#FUNCTION TO INITIALIZE DATABASE

def innit_database():
    
    global ref_global

    try:
        cred = credentials.Certificate(credentials_database)
        info = firebase_admin.initialize_app(cred, {'databaseURL': 'https://cms-rasp-default-rtdb.europe-west1.firebasedatabase.app'})
        print(f"Firebase Admin SDK initialized: {info.name}")
    except Exception as e:
        print(f"Error initializing Firebase Admin SDK: {e}")

    #firebase_admin.initialize_app(cred)
    
    #create the data for the sensors
    sensors_notification = {"Motion": 0, "Magnetic" : 0, "Button" : 0, "Relay": 0}
    sensors_data = [dict_sensors("Test Motion","Motion", "Hallway"), dict_sensors("Test Magnetic", "Magnetic", "Front Door"), dict_sensors("Test Button", "Button", "Hallway"), dict_sensors("Stove Relay", "Relay", "Kitchen")]

    #create the separated files for each data
    # Create the separated files for each data
    ref = db.reference("/")

    ref_global = ref

    ref.child("Notifications").set(sensors_notification)
    ref.child("Data").set(sensors_data)
    #ref.child("Logs")

    return ref

#----------------------------------

#FUNCTIONS TO GET DATA FROM DATABASE

def get_all_data():
    #app = firebase_admin.get_app()

    return ref_global.get()

def get_data_path(path):

    #app = firebase_admin.get_app()
    #cred = credentials.Certificate('database.json')
    #firebase_admin.initialize_app(cred, {'databaseURL': 'https://cms-rasp-default-rtdb.europe-west1.firebasedatabase.app'})

    result = db.reference(path).get()
    return str(result)

#----------------------------------

#FUNCTIOS TO SET DATA IN DATABASE

def set_operation(ref, key, value):         #go to desired location 'ref' , a specified 'key' with the desired 'value'
    
    #app = firebase_admin.get_app()
    #cred = credentials.Certificate('database.json')
    #firebase_admin.initialize_app(cred, {'databaseURL': 'https://cms-rasp-default-rtdb.europe-west1.firebasedatabase.app'})

    ref.child(key).set(value)
    return ref.get()


def update_flags(sensor_type, new_state):

    #app = firebase_admin.get_app()
    #cred = credentials.Certificate('database.json')
    #firebase_admin.initialize_app(cred, {'databaseURL': 'https://cms-rasp-default-rtdb.europe-west1.firebasedatabase.app'})

    ref = db.reference("/Notifications")
    ref.child(sensor_type).set(new_state)
    print(f"Updated {sensor_type} flag to {new_state}")


#def update_logs(sensor_type, date):
#    path = {"Data" : sensor_type}
#    data = get_data_path(path)
#
#    ref = db.reference("/Logs")
#    ref.child(date).set(data)

#----------------------------------

#FUNCTIONS FOR USER CONFIGURATION

def new_user(mail, password, house_code):
    user_data = {
        "email": mail,
        "password": password,
        "house_code": house_code,
        # Add other user-related data as needed
    }
    
    # create a new user under "Users" in the database
    ref = db.reference("/Users")
    ref.push().set(user_data)
    
    return f"New user created: {user_data}"

def add_user(mail, password):
    user_data = {
        "email": mail,
        "password": password,
        # Add other user-related data as needed
    }
    
    # add a user under "Users" in the database
    ref = db.reference("/Users")
    ref.push().set(user_data)
    
    return f"User added: {user_data}"

def update_user(mail, password):
    user_data = {
        "email": mail,
        "password": password,
        # Add other user-related data as needed
    }
    
    # update a user under "Users" in the database
    ref = db.reference("/Users")
    # Replace "existing_user_key" with the key of the user you want to update
    ref.child("existing_user_key").update(user_data)
    
    return f"User updated: {user_data}"

