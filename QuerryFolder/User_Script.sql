CREATE TABLE Member (
    member_id VARCHAR(10) PRIMARY KEY,
    CHECK (member_id REGEXP '^[A-Za-z0-9]{1,10}$')
);

CREATE TABLE RunningRecord (
    record_id INT AUTO_INCREMENT PRIMARY KEY,
    member_id VARCHAR(10) NOT NULL,
    run_time DOUBLE NOT NULL,
    avg_speed DECIMAL(3,1) NOT NULL,
    distance DECIMAL(6,3) NOT NULL,
    calorie DECIMAL(5,1) NOT NULL,
    record_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (member_id) REFERENCES Member(member_id)
);

INSERT INTO Member (member_id) VALUES (3);

SELECT *
FROM Book;

select count(*)
from Book
where bookid =1;