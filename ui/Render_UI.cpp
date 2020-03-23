#include "Render_UI.h"
#include "ui_Render_UI.h"
#include "qfiledialog.h"
#include "qlineedit.h"
#include "qmessagebox.h"
#include "x3dParser.h"
#include "Renderer.h"
#include "BasicShape.h"
#include "Box.h"
#include "qtextstream.h"

QString *curr_dirr;
Parser * p = nullptr;
uint8_t* image = (uint8_t*)malloc(1920 * 1080 * 3 * sizeof(uint8_t));
int32_t* ids = (int32_t*)malloc(1920 * 1080 * sizeof(int32_t));

Render_UI::Render_UI(QWidget *parent) : QMainWindow(parent)
{
	curr_dirr = new QString();
	ui.setupUi(this);
	connect(ui.open_button, &QPushButton::clicked, this, &Render_UI::browse);
	connect(ui.render_button, &QPushButton::clicked, this, &Render_UI::render);
	connect(ui.save_button, &QPushButton::clicked, this, &Render_UI::save);
	QPixmap myPixmap("waiting.png");
	ui.image_lable->setPixmap(myPixmap);
	ui.image_lable->setScaledContents(true);

}

void Render_UI::browse()
{
	*curr_dirr = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("*.x3d *.xml *.html"));
	p = new Parser(curr_dirr->toStdString());
	if (!(p->hasNext()))
	{
		p = nullptr;
		QMessageBox::critical(nullptr, "Incorrect file", "File is incorrect or we can't render this scene");
	}
	else QMessageBox::information(nullptr, "Successfull", "File was added");
}

void Render_UI::render()
{
	if (curr_dirr == nullptr) {
		QMessageBox::critical(nullptr, "Incorrect file", "Choose file with X3D scene");
		return;
	}
	if (p == nullptr && curr_dirr != nullptr)
		p = new Parser(curr_dirr->toStdString());
	
	if (!(p->hasNext()))
	{
		p = nullptr;
		QMessageBox::critical(nullptr, "Incorrect file", "File is incorrect or we can't render this scene");
		return;
	}


	CameraParam param = CameraParam(4, 90.0f);
	std::vector<BasicShape*> shapes;

	while (p->hasNext()) {
		shapes.push_back(p->NextElem());
	}

	Renderer rd = Renderer(param);
	rd.initScene(shapes);
	rd.cam.setLocation(Vector3(10, -15, 2));
	rd.cam.Rotate(Quaternion(rd.cam.Z, acosf(-1) / 12));
	freopen("out.ppm", "wb", stdout);
	uint8_t header[] = "P6 1920 1080 255\n";
	header[17] = 10;
	std::cerr << rd.scene->getSize() << std::endl;

	rd.CPURenderPPMImage(&image, &ids, 1920, 1080);
	rd.scene->printBenchmark();

	fwrite(header, sizeof(uint8_t), sizeof(header) - 1, stdout);
	fwrite(image, sizeof(uint8_t), 1920 * 1080 * 3, stdout);
	freopen("im.txt", "w", stdout);

	for (int i = 0; i < 1080; i++)
	{
		for (int j = 0; j < 1920; j++)
		{
			std::cout << ids[i * 1920 + j] << " ";
		}
		std::cout << "\n";
	}
	free(image);
	free(ids);
	QPixmap myPixmap("out.ppm");
	ui.image_lable->setPixmap(myPixmap);
	
	p = nullptr;

}

void Render_UI::save() 
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Сохранить файл"), "",
		tr(".ppm"));
	if (fileName != "") {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox msgBox; msgBox.setText("Не могу записать файл"); msgBox.exec();
			//или как выше, коротко QMessageBox::critical...
		}
		else {
			
			QByteArray fileContents = QByteArray((char*)image, 1080 * 1920);
			file.write(fileContents);
			file.close();
		}
	}
}
