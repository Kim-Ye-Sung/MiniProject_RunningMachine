from flask import Flask, request, jsonify
import mysql.connector
from config import DB_CONFIG, SERVER_HOST, SERVER_PORT

app = Flask(__name__)


def get_connection():
    conn = mysql.connector.connect(
        host=DB_CONFIG["host"],
        user=DB_CONFIG["user"],
        password=DB_CONFIG["password"],
        database=DB_CONFIG["database"]
    )

    cursor = conn.cursor()
    cursor.execute("SET time_zone = '+09:00'")
    cursor.close()

    return conn


@app.route("/member/exists", methods=["POST"])
def member_exists():
    data = request.get_json()

    if not data or "member_id" not in data:
        return jsonify({
            "success": False,
            "message": "member_id is required"
        }), 400

    member_id = data["member_id"]

    try:
        conn = get_connection()
        cursor = conn.cursor()

        query = "SELECT COUNT(*) FROM Member WHERE member_id = %s"
        cursor.execute(query, (member_id,))
        count = cursor.fetchone()[0]

        cursor.close()
        conn.close()

        return jsonify({
            "success": True,
            "exists": count >= 1
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "message": str(e)
        }), 500


@app.route("/member/create", methods=["POST"])
def create_member():
    data = request.get_json()

    if not data or "member_id" not in data or "password" not in data:
        return jsonify({
            "success": False,
            "message": "member_id and password are required"
        }), 400

    member_id = data["member_id"]
    password = data["password"]

    try:
        conn = get_connection()
        cursor = conn.cursor()

        sql = "INSERT INTO Member (member_id, Password) VALUES (%s, %s)"
        cursor.execute(sql, (member_id, password))
        conn.commit()

        cursor.close()
        conn.close()

        return jsonify({
            "success": True,
            "message": "member created"
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "message": str(e)
        }), 500


@app.route("/member/check_password", methods=["POST"])
def check_password():
    data = request.get_json()

    if not data or "member_id" not in data or "password" not in data:
        return jsonify({
            "success": False,
            "message": "member_id and password are required"
        }), 400

    member_id = data["member_id"]
    password = data["password"]

    try:
        conn = get_connection()
        cursor = conn.cursor()

        sql = "SELECT Password FROM Member WHERE member_id = %s"
        cursor.execute(sql, (member_id,))
        row = cursor.fetchone()

        cursor.close()
        conn.close()

        if row is None:
            return jsonify({
                "success": True,
                "match": False
            }), 200

        db_password = row[0]

        return jsonify({
            "success": True,
            "match": db_password == password
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "message": str(e)
        }), 500


@app.route("/record/save", methods=["POST"])
def record_save():
    data = request.get_json()

    required_keys = ["member_id", "run_time", "avg_speed", "distance", "calorie"]
    if not data or any(key not in data for key in required_keys):
        return jsonify({
            "success": False,
            "message": "missing required fields"
        }), 400

    try:
        conn = get_connection()
        cursor = conn.cursor()

        query = """
            INSERT INTO RunningRecord
            (member_id, run_time, avg_speed, distance, calorie)
            VALUES (%s, %s, %s, %s, %s)
        """
        cursor.execute(
            query,
            (
                data["member_id"],
                data["run_time"],
                data["avg_speed"],
                data["distance"],
                data["calorie"]
            )
        )
        conn.commit()

        cursor.close()
        conn.close()

        return jsonify({
            "success": True,
            "message": "record saved"
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "message": str(e)
        }), 500


@app.route("/record/inquiry", methods=["POST"])
def record_inquiry():
    data = request.get_json()

    required_keys = ["member_id", "start_date", "end_date"]
    if not data or any(key not in data for key in required_keys):
        return jsonify({
            "success": False,
            "message": "missing required fields"
        }), 400

    try:
        conn = get_connection()
        cursor = conn.cursor(dictionary=True)

        query = """
            SELECT
                record_date,
                run_time,
                avg_speed,
                distance,
                calorie
            FROM RunningRecord
            WHERE member_id = %s
              AND record_date BETWEEN %s AND %s
            ORDER BY record_date DESC
        """

        start_datetime = data["start_date"] + " 00:00:00"
        end_datetime = data["end_date"] + " 23:59:59"

        cursor.execute(query, (data["member_id"], start_datetime, end_datetime))
        rows = cursor.fetchall()

        result = []
        for row in rows:
            total_seconds = int(row["run_time"])

            hour = total_seconds // 3600
            minute = (total_seconds % 3600) // 60
            second = total_seconds % 60

            run_time_text = f"{hour:02d}:{minute:02d}:{second:02d}"

            result.append({
                "record_date": row["record_date"].strftime("%Y-%m-%d %H:%M"),
                "run_time": run_time_text,
                "avg_speed": float(row["avg_speed"]),
                "distance": float(row["distance"]),
                "calorie": float(row["calorie"])
            })

        cursor.close()
        conn.close()

        return jsonify({
            "success": True,
            "records": result
        }), 200

    except Exception as e:
        return jsonify({
            "success": False,
            "message": str(e)
        }), 500


if __name__ == "__main__":
    app.run(host=SERVER_HOST, port=SERVER_PORT, debug=True)