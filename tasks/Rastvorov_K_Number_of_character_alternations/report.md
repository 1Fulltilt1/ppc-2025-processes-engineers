Use the following skeleton as a starting point for your ``report.md``. Keep file paths relative to the task directory (for images/data).

.. code-block:: markdown

   # <Task Нахождение числа чередований знаков значений соседних элементов вектора>
   
   - Student: <Растворов Кирилл Евгеньевич>, group <3823Б1ПР3>
   - Technology: <SEQ | MPI >
   - Variant: <5>
   
   ## 1. Introduction
   Задача состоит в нахождении числа знаков значений соседних элементов вектора. Цель работы - разработать SEQ и MPI реализациюб и провести функциональную и производительную оценку, сравнив результаты с использованием тестов PPC.
   
   ## 2. Problem Statement
   Formal task definition, input/output format, constraints.
   Нам дан вектор н-ой длины элементы его определяются по идексу и требуется определьить число знака между последовательными не нулевыми элементами.
   Например N = 7
   i: 0 1 2 3 4 5 6 -> v: 0 -1 1 -1 1 1
   на вход мы получаем длину вектора, а сам вектор мы восстанавливаем с помощи индекса и формулы в функции GetElement. На выходе мы получим целое число
   ## 3. Baseline Algorithm (Sequential)
   Describe the base algorithm with enough detail to reproduce.
   Последовательный алгоритм пройдет по вектору от 0 до n-1, где n - длинна вектора
   1. Вычесляем виртуальное значение вектора v[i] по формуле a i-тый = 0, если i/5 = 0
                    +1  если i/5 не равно 0 и i/2 = 0
                    -1  если i/5 и i/2 не равно 0
                    (Все деления mod)
   2. пропускаем 0 v[i] == 0;
   3. Сравниваем знак текущего не нулевого с предедыщем не нулевым и если знак разный count++
   4. count - наше искомое число

   ## 4. Parallelization Scheme
   - For MPI: data distribution, communication pattern/topology, rank roles.
   - For threads: decomposition, scheduling, synchronization.
   Diagrams or short pseudocode are welcome.
   Диапазон индексов (0, N) распределяется между процессами равномерно. Процессам 0 - rem-1 выделяются блоки в размере (base+1), другим блоки base
   где base = n/p, rem = n%p
   Каждый процесс будет обрабатывать свой диапазон:
    1.  local_count - число чередований
    2. first_sign - знак первого не нулевого элемента
    3. last_sign - соотвественно последнего\
    4. передаем результаты на rank 0 с помощью MPI_Gather
    5. Собираем результат в root
        1. Складываем все local_count
        2. Анализируем стыки. (Например last_sign[p-1!= first_sign[p] - оба не 0 - значит чередование)
        3. подводим итог
    Топология линейная, а передаем по моделе master - worker
   ## 5. Implementation Details
   - Code structure (files, key classes/functions)
   - Important assumptions and corner cases
   - Memory usage considerations
    common/include/common.hpp        — типы InType / OutType
    seq/src/ops_seq.cpp              — SEQ реализация RunImpl
    mpi/src/ops_mpi.cpp              — MPI реализация RunImpl
    tests/functional/main.cpp        — функциональные тесты PPC
    tests/performance/main.cpp       — performance тесты PPC
    RastvorovKNumberAfCharacterAlternationsSEQ — последовательная реализация,
    RastvorovKNumberAfCharacterAlternationsMPI — MPI-реализация.
    Оба класса наследуются от базового класса задач PPC и переопределяют стандартные методы:

    ValidationImpl() — проверка входных данных,
    PreProcessingImpl() — подготовка,
    RunImpl() — основная логика,
    PostProcessingImpl() — финальная стадия (в этой задаче почти пустая).
    Типы:
    InType — входное значение (длина вектора N),
    OutType — выход (количество чередований, целое число).

   ## 6. Experimental Setup
   - Hardware/OS: CPU model, cores/threads, RAM, OS version
   - Toolchain: compiler, version, build type (Release/RelWithDebInfo)
   - Environment: PPC_NUM_THREADS / PPC_NUM_PROC, other relevant vars
   - Data: how test data is generated or sourced (relative paths)
   
    CPU: 8 vCPU (Docker/DevContainer)
    RAM: 16 GB
    OS: Ubuntu 22.04 (WSL2 / Container)
    MPI: OpenMPI 4.1.x
    Компилятор: g++ 11, -O3, -march=x86-64
    Сборка: Release
    Данные не загружаются, а определяются по формуле в зависимости от индекса

    PPC_NUM_PROC = 4
    PPC_NUM_THREADS = 1 (не используется)
    USE_FUNC_TESTS = ON
    USE_PERF_TESTS = ON

   ## 7. Results and Discussion
   
   ### 7.1 Correctness
   Briefly explain how correctness was verified (reference results, invariants, unit tests).
   
   ### 7.2 Performance
   Present time, speedup and efficiency. Example table:
   
   | Mode            |Time, s | Speedup | Efficiency |
   |-------------    |--------|---------|------------|
   | SEQ, pipeline   | 0.00050| 1.00    |    100%    |
   | MPI(4), pipeline| 0.00014| 3.5     |   87.5%    |
   | SEQ, task_run   | 0.00051| 1       |    100%    |
   |MPI(4), task_run | 0.00014| 3.6     |     90%    |
   
   Optionally add plots (use relative paths), and discuss bottlenecks and scalability limits.
   MPi в среднем дает 3.5 ускорение на 4х процессах, теоретически это максимум 
   из-за малого размера сообщений получается наибольший процент эффиктивности
   Скейлинг ограничен длинной вектора

   ## 8. Conclusions
   Summarize findings and limitations.
    Реализован корректный последовательный и MPI-алгоритм для подсчёта количества чередований символов.
    Использована оптимальная модель MPI с минимальным взаимодействием.
    Все функциональные и производительные тесты были успешно завершены.
    MPI показал ускорение примерно в 3,5 раза на 4 процессах.

    Ограничения:
    При очень малых N ускорение не имеет смысла (накладные расходы MPI).
    Распараллеливание эффективно только для достаточно больших векторов.
   ## 9. References
   1. <Article/Book/Doc URL>
   2. <Another source>
    1. OpenMPI Documentation — https://www.open-mpi.org/doc/
    2. PPC Parallel Programming Course — https://learning-process.github.io/parallel_programming_course/
    3. GTest Framework — https://github.com/google/googletest
    4. Статья https://www.cyberforum.ru/cpp-beginners/thread597195.html
    5. Книга "Параллельное программированиена C++ в действии" автор: ANTHONY WILLIAMS https://dodo.inm.ras.ru/konshin/HPC/bib/HPC-cxx11-book.pdf
    6. Физическая книга С.Макконнел "Совершенный код. Практическое руководство по разработке программного Обеспечения" 2014г.

    
   ## Appendix (Optional)
   ```cpp
   // Short, readable code excerpts if needed
   ```