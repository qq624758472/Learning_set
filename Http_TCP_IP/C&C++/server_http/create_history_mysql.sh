#!/bin/bash

HOSTNAME="localhost"
USERNAME="root"
PASSWORD="root"

DBNAME="operating_info"
TABLENAME="operating_history_info"

show_db="show databases"

create_db_sql="create database IF NOT EXISTS ${DBNAME}"
mysql -h${HOSTNAME} -u${USERNAME} -p${PASSWORD} -e "${create_db_sql}"

create_table_sql="create table IF NOT EXISTS ${TABLENAME} (  operating_person varchar(40), operating_name  varchar(20), operating_time varchar(40), operating_source varchar(40), operating_target varchar(40) , operating_schedule int(11) default -1)"
mysql -h${HOSTNAME} -u${USERNAME} -p${PASSWORD}  -D ${DBNAME} -e "${create_table_sql}"

# mysql -h${HOSTNAME} -u${USERNAME} -p${PASSWORD} -e "${show_db}"

# select_sql="select * from ${TABLENAME}"
# mysql -h${HOSTNAME} -u${USERNAME} -p${PASSWORD} ${DBNAME} -e "${select_sql}"


