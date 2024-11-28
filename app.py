from flask import Flask
from flask import render_template, Response, request
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import inspect, create_engine

HOST = "127.0.0.1"
PORT =  5555

PROJECT_NAME     = "Krypton"
STATIC_FOLDER    = "static"
TEMPLATES_FOLDER = "templates"
DATA_BASE        = "test.db"
TABLE_NAME       = "users"

app = Flask(PROJECT_NAME, static_url_path="", static_folder=STATIC_FOLDER, template_folder=TEMPLATES_FOLDER)

app.config["SQLALCHEMY_DATABASE_URI"]        = "sqlite:///" + DATA_BASE
app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False

db = SQLAlchemy(app)

class User(db.Model):
    __tablename__ = TABLE_NAME
    id       = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False)
    password = db.Column(db.String(80), nullable=False)
    email    = db.Column(db.String(120), unique=True, nullable=False)

    def __repr__(self):
        return f"<User {self.username}>"

engine = create_engine('sqlite://')

if not inspect(engine).has_table(TABLE_NAME):
   with app.app_context():
        db.create_all()

@app.route("/")
def main():
    return render_template("index.html")

@app.route("/<path>", methods=["GET"])
def just_render(path):
    if path in ["index.html", "sighin.html", "sighup.html"]:
        return render_template(path)

@app.route("/signup_post", methods=["POST"])
def sign_up():
    username = request.form["username"]
    password = request.form["password"]
    email    = request.form["email"]
   
    users = User.query.all()
    
    if len(users) == 0:
        id = 1;
    else:
        id = users[len(users)-1].id + 1 
    
    flag = False
    for user in users:
        if user.username == username or user.email == email:
            flag = True
            break
    
    if not flag:
        new_user = User(id=id, username=username, password=password, email=email)
        db.session.add(new_user)
        db.session.commit()
        return f"User {username} added successfully!"
    else:
        return f"User {username} allready exists!"

@app.route("/signin_post", methods=["POST"])
def sign_in():
    username = request.form["username"]
    password = request.form["password"]
   
    users = User.query.all()

    for user in users:
        if user.username == username:
            if user.password == password:
                return f"User {username} access granted!"
    else:
        return f"User {username} access denied!"

if __name__ == "__main__":
    app.run(debug=True, host=HOST, port=PORT)
