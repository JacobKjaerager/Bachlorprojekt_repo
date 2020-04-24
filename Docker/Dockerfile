FROM python:3.6

LABEL maintainer "Jacob Kjærager <201611661@post.au.dk>"

WORKDIR /

COPY requirements.txt /

RUN pip install -r /requirements.txt

COPY ./ ./

EXPOSE 8050

CMD ["python", "./Dash_server_and_gui.py"]