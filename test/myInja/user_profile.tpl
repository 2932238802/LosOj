<h1>User Profile: {{ user.name }}</h1>
<p>Email: {{ user.email }}</p>
<p>Age: {{ user.age }}</p>

<h2>Skills:</h2>
<ul>
{% for skill in user.skills %}
<li>{{ skill }}</li>
{% endfor %}
</ul>
