-- I. СОЗДАНИЕ СТРУКТУРЫ ТАБЛИЦ
-- ON UPDATE CASCADE для неизменяемых id, как и некоторые NOT NULL ограничения, оставлены для наглядности

BEGIN;

-- 1. Факультативы (id, название_факультатива).
-- Ключи: {id}
-- UNIQUE на Название_факультатива исключает дублирование названий факультативов с разными id
-- Запуск для разных контингентов также возможен через записи в "Семестровых_курсах"
CREATE TABLE факультативы (
    id 				      SERIAL PRIMARY KEY,
    название_факультатива VARCHAR(100) NOT NULL,

    CONSTRAINT uq_название_факультатива
		UNIQUE (название_факультатива)
);

-- 2. Компетенции_преподавателей (id_факультатива, id_преподавателя)
-- Ключи: {id_факультатива, id_преподавателя}
CREATE TABLE компетенции_преподавателей (
    id_факультатива  INT NOT NULL,
    id_преподавателя INT NOT NULL,
	
    CONSTRAINT pk_преподаватель_компетенции
		PRIMARY KEY (id_факультатива, id_преподавателя),

	-- Удаление факультатива снимает все компетенции
	CONSTRAINT fk_компетенции_факультатив 
		FOREIGN KEY (id_факультатива)
		REFERENCES факультативы(id)
		ON UPDATE CASCADE
		ON DELETE CASCADE
);

-- 3. Прохождения курсов (id_факультатива, id_студента, итоговая_оценка)
-- Ключи: {id_факультатива, id_студента}
CREATE TABLE прохождения_курсов (
    id_факультатива INT NOT NULL,
    id_студента     INT NOT NULL,
    итоговая_оценка INT,
	
    CONSTRAINT pk_факультатив_студент 
		PRIMARY KEY (id_факультатива, id_студента),

	-- Нельзя удалить факультатив, пока есть записанные студенты
    CONSTRAINT fk_прохождения_факультатив
		FOREIGN KEY (id_факультатива)
		REFERENCES факультативы(id)
		ON UPDATE CASCADE
		ON DELETE RESTRICT,

	-- NULL, чтобы студент мог быть записан на факультатив без оценки
    CONSTRAINT chk_прохождения_итоговая_оценка
		CHECK (итоговая_оценка IS NULL
			OR итоговая_оценка BETWEEN 2 AND 5)
);

-- 4. Виды_занятия (id, название_вида)
-- Ключи: {id}, {название_вида}
CREATE TABLE виды_занятия (
    id            SERIAL PRIMARY KEY,
    название_вида VARCHAR(50) NOT NULL,

    CONSTRAINT uq_виды_занятия_название
		UNIQUE (название_вида)
);

-- Наполнение справочника значениями
INSERT INTO виды_занятия (название_вида) VALUES
    ('Лекция'),
    ('Практика'),
    ('Лабораторная работа');


-- 5. Семестровые курсы (id, учебный_год, номер_семестра, id_факультатива)
-- Ключи: {id}, {учебный_год, номер_семестра, id_факультатива}
CREATE TABLE семестровые_курсы (
	id              SERIAL PRIMARY KEY,
	учебный_год	 	INT NOT NULL,
	номер_семестра  INT NOT NULL,
	id_факультатива INT NOT NULL,

	CONSTRAINT uq_семестровые_курсы_сост_ключ
        UNIQUE (учебный_год, номер_семестра, id_факультатива),

	CONSTRAINT chk_семестровые_курсы_год
        CHECK (учебный_год BETWEEN 2000 AND 2050),

	CONSTRAINT chk_семестровые_курсы_номер
        CHECK (номер_семестра IN (1, 2)),

	-- Запрет удаления, если факультатив уже запланирован к реализации в конкретных семестрах,
	-- чтобы удаление не случилось по цепочке для всех записей
	CONSTRAINT fk_семестровые_курсы_факультатив
		FOREIGN KEY (id_факультатива)
		REFERENCES факультативы(id)
		ON UPDATE CASCADE
		ON DELETE RESTRICT
);

-- 6. Студенты (id, фамилия, имя, отчество, телефон, адрес, минимальное_количество_предметов)
-- Ключи: {id}, {телефон}
CREATE TABLE студенты (
	id                               SERIAL PRIMARY KEY,
    фамилия                          VARCHAR(100) NOT NULL,
    имя                              VARCHAR(100) NOT NULL,
    отчество                         VARCHAR(100),
    телефон                          VARCHAR(20) NOT NULL,
    адрес                            VARCHAR(255),
    минимальное_количество_предметов INT NOT NULL DEFAULT 1,

	CONSTRAINT uq_студенты_телефон 
        UNIQUE (телефон),

    CONSTRAINT chk_студенты_телефон
        CHECK (телефон ~ E'^\\+?[0-9][0-9\\s\\-\\(\\)]{6,19}$'),

    CONSTRAINT chk_студенты_мин_предметы
        CHECK (минимальное_количество_предметов >= 0)
);

-- 7. Обучение студента (id, количество_часов, id_вида_занятия, id_семестрового_курса, id_преподавателя)
-- Ключи: {id}, {id_вида_занятия, id_семестрового_курса}
CREATE TABLE обучение_студента (
    id                    SERIAL PRIMARY KEY,
    количество_часов      INT NOT NULL,
    id_вида_занятия       INT NOT NULL,
    id_семестрового_курса INT NOT NULL,
    id_преподавателя      INT NOT NULL,

    CONSTRAINT uq_обучение_студента_сост_ключ
        UNIQUE (id_вида_занятия, id_семестрового_курса),

    CONSTRAINT chk_обучение_студента_часы
        CHECK (количество_часов > 0),

    CONSTRAINT fk_обучение_студента_вид
        FOREIGN KEY (id_вида_занятия)
        REFERENCES виды_занятия(id)
		ON UPDATE CASCADE
        ON DELETE RESTRICT,

    CONSTRAINT fk_обучение_студента_семкурс
        FOREIGN KEY (id_семестрового_курса)
        REFERENCES семестровые_курсы(id)
		ON UPDATE CASCADE
        ON DELETE CASCADE
);

-- 8. Экзаменационные ведомости (id, дата_создания, id_семестрового_курса, id_преподавателя)
-- Ключи: {id}, {id_семестрового_курса, дата_создания}
CREATE TABLE экзаменационные_ведомости (
    id                    SERIAL PRIMARY KEY,
    дата_создания         DATE NOT NULL DEFAULT CURRENT_DATE,
    id_семестрового_курса INT NOT NULL,
    id_преподавателя      INT NOT NULL,

    CONSTRAINT uq_экз_ведомости_сост_ключ
        UNIQUE (id_семестрового_курса, дата_создания),

    CONSTRAINT fk_экз_ведомости_семкурс
        FOREIGN KEY (id_семестрового_курса)
        REFERENCES семестровые_курсы (id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

-- 9. Записи в ведомостях (id_ведомости, id_студента, оценка)
-- Ключи: {id_ведомости, id_студента}
CREATE TABLE записи_в_ведомостях (
    id_ведомости INT NOT NULL,
    id_студента  INT NOT NULL,
    оценка       INT NOT NULL,

    CONSTRAINT pk_записи_в_ведомостях
        PRIMARY KEY (id_ведомости, id_студента),

    CONSTRAINT chk_записи_в_ведомостях_оценка
        CHECK (оценка BETWEEN 2 AND 5),

    CONSTRAINT fk_записи_экз_ведомость
        FOREIGN KEY (id_ведомости)
        REFERENCES экзаменационные_ведомости(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE,

    CONSTRAINT fk_записи_в_ведомостях_студент
        FOREIGN KEY (id_студента)
        REFERENCES студенты(id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT
);

-- 10. Преподаватели (id, фамилия, имя, отчество, телефон, должность, id_пользователя)
-- Ключи: {id}, {телефон}
CREATE TABLE преподаватели (
    id               SERIAL PRIMARY KEY,
    фамилия          VARCHAR(100) NOT NULL,
    имя              VARCHAR(100) NOT NULL,
    отчество         VARCHAR(100),
    телефон          VARCHAR(20) NOT NULL,
    должность        VARCHAR(100),
    id_пользователя  INT NOT NULL,

    CONSTRAINT uq_преподаватели_телефон 
        UNIQUE (телефон),

    CONSTRAINT uq_преподаватели_пользователь 
        UNIQUE (id_пользователя),

    CONSTRAINT chk_преподаватели_телефон
        CHECK (телефон ~ E'^\\+?[0-9][0-9\\s\\-\\(\\)]{6,19}$')
);

-- 11. Пользователи (id, логин, хэш_пароля, электронная_почта)
-- Ключи: {id}, {логин}, {электронная_почта}
CREATE TABLE пользователи (
    id                SERIAL PRIMARY KEY,
    логин             VARCHAR(70) NOT NULL,
    хэш_пароля        VARCHAR(70) NOT NULL,
    электронная_почта VARCHAR(150) NOT NULL,

    CONSTRAINT uq_пользователи_логин  
        UNIQUE (логин),
		
    CONSTRAINT uq_пользователи_email  
        UNIQUE (электронная_почта),

    CONSTRAINT chk_пользователи_логин
        CHECK (LENGTH(логин) >= 6)
);

-- 12. Роли_пользователей (id_пользователя, id_роли)
-- Ключи: {id_пользователя, id_роли}
CREATE TABLE роли_пользователей (
    id_пользователя INT NOT NULL,
    id_роли         INT NOT NULL,

    CONSTRAINT pk_роли_пользователей
        PRIMARY KEY (id_пользователя, id_роли),

    CONSTRAINT fk_роли_пользователей_пользователь
        FOREIGN KEY (id_пользователя)
        REFERENCES пользователи(id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
);

-- 13. Роли (id, название_роли)
-- Ключи: {id}, {название_роли}
CREATE TABLE роли (
    id            SERIAL PRIMARY KEY,
    название_роли VARCHAR(50) NOT NULL,

    CONSTRAINT uq_роли_название 
        UNIQUE (название_роли)
);

-- Наполнение справочника ролей
INSERT INTO роли (название_роли) VALUES
    ('Суперпользователь'),
    ('Методист'),
	('Преподаватель');


-- II. ВНЕШНИЕ КЛЮЧИ НА ТАБЛИЦЫ, ОПРЕДЕЛЕННЫЕ ПОЗЖЕ ТОЙ, НА КОТОРУЮ ЕСТЬ ССЫЛКА
-- 2 -> 10: компетенции_преподавателей -> преподаватели
ALTER TABLE компетенции_преподавателей
    ADD CONSTRAINT fk_компетенции_преподаватель
        FOREIGN KEY (id_преподавателя)
        REFERENCES преподаватели(id)     	
        ON DELETE CASCADE;					-- Удаление преподавателя снимает все компетенции

-- 3 -> 6: прохождения_курсов -> студенты
ALTER TABLE прохождения_курсов
	ADD CONSTRAINT fk_прохождения_студент
		FOREIGN KEY (id_студента)
		REFERENCES студенты(id)
		ON DELETE RESTRICT;				    -- Запрет удаления студента с имеющейся историей прохождения для хранения архивной отчетности

-- 7 -> 10: обучение_студента -> преподаватели
ALTER TABLE обучение_студента
    ADD CONSTRAINT fk_обучение_студента_преподаватель
        FOREIGN KEY (id_преподавателя)
        REFERENCES преподаватели(id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT;

-- 8 -> 10: экзаменационные_ведомости -> преподаватели
ALTER TABLE экзаменационные_ведомости
    ADD CONSTRAINT fk_экз_ведомости_преподаватель
        FOREIGN KEY (id_преподавателя)
        REFERENCES преподаватели(id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT;

-- 10 -> 11: преподаватели -> пользователи
ALTER TABLE преподаватели
    ADD CONSTRAINT fk_преподаватели_пользователь
        FOREIGN KEY (id_пользователя)
        REFERENCES пользователи(id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT;

-- 12 -> 13: роли_пользователей -> роли
ALTER TABLE роли_пользователей
    ADD CONSTRAINT fk_роли_пользователей_роль
        FOREIGN KEY (id_роли)
        REFERENCES роли(id)
        ON UPDATE CASCADE
        ON DELETE RESTRICT;

COMMIT;