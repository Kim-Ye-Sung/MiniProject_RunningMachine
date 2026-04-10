CREATE DATABASE RunRecordDB;  # 사용할 db 생성

CREATE USER 'KYS'@'%' IDENTIFIED BY 'KYS123456';  # db를 사용할 유저 생성	


GRANT ALL PRIVILEGES ON RunRecordDB.* TO 'KYS'@'%';	# 유저가 DB에 접속할수있게 접속 권한을 부여
FLUSH PRIVILEGES;

DROP USER IF EXISTS 'KYS'@'%';