-- Синхронизация итоговой оценки в прохождения_курсов
-- Итоговая оценка = последняя оценка студента
BEGIN;

CREATE OR REPLACE FUNCTION sync_final_grade()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
DECLARE
    v_elective_id INT;
    v_latest_grade INT;
BEGIN
    SELECT sk.id_факультатива
      INTO v_elective_id
      FROM экзаменационные_ведомости ev
      JOIN семестровые_курсы sk ON ev.id_семестрового_курса = sk.id
     WHERE ev.id = NEW.id_ведомости;

    IF v_elective_id IS NULL THEN
        RETURN NEW;
    END IF;

    SELECT z.оценка
      INTO v_latest_grade
      FROM записи_в_ведомостях z
      JOIN экзаменационные_ведомости ev ON z.id_ведомости = ev.id
      JOIN семестровые_курсы sk ON ev.id_семестрового_курса = sk.id
     WHERE sk.id_факультатива = v_elective_id
       AND z.id_студента = NEW.id_студента
     ORDER BY ev.дата_создания DESC, ev.id DESC
     LIMIT 1;

    IF v_latest_grade IS NOT NULL THEN
        UPDATE прохождения_курсов
           SET итоговая_оценка = v_latest_grade
         WHERE id_факультатива = v_elective_id
           AND id_студента = NEW.id_студента;
    END IF;

    RETURN NEW;
END;
$$;

DROP TRIGGER IF EXISTS trg_sync_final_grade ON записи_в_ведомостях;

CREATE TRIGGER trg_sync_final_grade
    AFTER INSERT OR UPDATE OF оценка
    ON записи_в_ведомостях
    FOR EACH ROW
    EXECUTE PROCEDURE sync_final_grade();

COMMIT;
