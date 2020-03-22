#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Render_UI.h"

class Render_UI : public QMainWindow
{
	Q_OBJECT

public:
	Render_UI(QWidget *parent = Q_NULLPTR);

public slots:
    void browse();
	void render();
	void save();

private:
	Ui::Render_UIClass ui;
};
