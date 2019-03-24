<h1>BardinaSPL</h1>
<h2>Подключение удалённого репозитория и загрузка файлов</h2>
<ol>
<li>Правой кнопкой в папке → Git Bash Here
<p><font face = Courier New>git remote add <i>Name_link</i> https://github.com/group184311/BardinaSPL.git</p></font>
</li>
<li>
<p><font face = Courier New>git remote -v</font></p>
<i>Появится список подключенных удалённых репозиториев</i>
</li>
<li>Загружаем файлы в удалённый репозиторий
<p><font face = Courier New>git push <i>Name_link</i> master</font></p>
</li>
</ol>
<h3>Ветка master загружена!</h3>
<h2>Выгрузка файлов из удалённого репозитория для начала работы на новом месте</h2>
<ol>
<li>Создаём папку в workspace с названием Lab3</li>
<li></li>
  <ol>
  <li>Правой кнопкой в папке → Git Bash Here
  <p><font face = Courier New>git config --global user.name <i> Login </i> <br>
  git config --global user.email <i> E-mail </i> <br>
  git config --list </font></p>
  <i>Проверяем, что появились глобальные имя и e-mail </i></li>
  <li>Инициализируемся
  <p><font face = Courier New>git init</font></p></li>
  <li>Клонируем проект в папку
  <p><font face = Courier New>git clone https://github.com/group184311/BardinaSPL.git master </font></p></li>
  </ol>
<li></li>
  <ol>
  <li>Заходим в Eclipse</li>
  <li>Создаём проект C project → Executable → Ac6 STM32 MCU GCC</li>
  <li>Имя проекта, как название папки - Lab3. Далее. Далее</li>
  <li>Выбираем тип платы → STM32F1 → Create new custom board</li>
    <ol>
    <li>Любое имя</li>
    <li>MCU - STM32F103C8Tx</li>
    </ol>
  <li>Далее</li>
  <li>Выбираем Firmware configuration → Standard Peripheral Library (StdPeriph). Готово</li>
  </ol>
<li><b>После создания проекта зайти в папку Lab3 → startup и удалить создавшийся файл startup_stm32.s</b>
</ol>
<h3>Проект готов к дальнейшей работе!</h3>
<h2>Выгрузка файлов из удалённого репозитория для продолжения работы</h2>
<i>Если Вы загрузили на удалённый репозиторий какое-то изменения с другого компьютера и хотите продолжить работу с конечной точки данной ветки на своём компьютере, на котором уже подклчен данный удалённый репозиторий</i><br>
<br>
<p><font face = Courier New>git pull <i>Name_link</i> master</font></p>
