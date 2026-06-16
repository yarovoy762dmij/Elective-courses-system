CREATE OR REPLACE VIEW view_course_details AS
WITH course_teachers AS (
    SELECT DISTINCT ON (id_семестрового_курса)
           id_семестрового_курса,
           id_преподавателя
    FROM обучение_студента
)
SELECT
    sc.id,
    sc.учебный_год,
    sc.номер_семестра,
    sc.id_факультатива,
    COALESCE(SUM(CASE WHEN lt.id = 1 THEN os.количество_часов END), 0) AS часы_лекций,
    COALESCE(SUM(CASE WHEN lt.id = 2 THEN os.количество_часов END), 0) AS часы_практик,
    COALESCE(SUM(CASE WHEN lt.id = 3 THEN os.количество_часов END), 0) AS часы_лабораторных,
    p.фамилия || ' ' || p.имя AS преподаватель
FROM семестровые_курсы sc
LEFT JOIN обучение_студента os ON sc.id = os.id_семестрового_курса
LEFT JOIN виды_занятия lt ON os.id_вида_занятия = lt.id
LEFT JOIN course_teachers ct ON ct.id_семестрового_курса = sc.id
LEFT JOIN преподаватели p ON p.id = ct.id_преподавателя
GROUP BY sc.id, sc.учебный_год, sc.номер_семестра, sc.id_факультатива, p.фамилия, p.имя
ORDER BY sc.id;