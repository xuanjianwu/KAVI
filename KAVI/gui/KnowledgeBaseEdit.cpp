#include "KnowledgeBaseEdit.h"
#include "KnowledgeBaseClassEditDialog.h"
#include "KnowledgeBasePredicateEditDialog.h"

KnowledgeBaseEdit::KnowledgeBaseEdit(KAVIClassKB *classKB, KAVIPredicateKB *predicateKB, QWidget *parent) :
    QDialog(parent), ui(new Ui::KnowledgeBaseEdit)
{
    ui->setupUi(this);

    nameChecker.setPattern(QString("^([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|[-_]){,%1}$").arg(MAX_LABEL_LENGTH));

    this->classKB = classKB;
    this->predicateKB = predicateKB;

    loadDataFromBase(classKB, ui->classList);
    loadDataFromBase(predicateKB, ui->predicateList);

    connect(ui->classList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editBase(QListWidgetItem*)));
    connect(ui->predicateList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(editBase(QListWidgetItem*)));
}

KnowledgeBaseEdit::~KnowledgeBaseEdit()
{
    delete ui;
}

void KnowledgeBaseEdit::loadDataFromBase(KAVIKB *base, QListWidget *list)
{
    list->clear();
    QStringList tmpList = base->getData();
    qSort(tmpList.begin(), tmpList.end());
    list->addItems(tmpList);
}

void KnowledgeBaseEdit::editBase(QListWidgetItem *item)
{
    if (ui->tabWidget->currentIndex() == TAB_INDEX_CLASS)
    {
        QString oldClassName = item->text();
        KnowledgeBaseClassEditDialog *dialog = new KnowledgeBaseClassEditDialog(oldClassName, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newClassName = dialog->className();
            QStringList definedClasses = classKB->getData();

            if ( newClassName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Class name can't be empty."));
                return;
            }

            if ( definedClasses.contains(newClassName, Qt::CaseInsensitive) )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Class name must be unique."));
                return;
            }

            if ( newClassName.toLower() == "object" )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                return;
            }

            if ( !nameChecker.exactMatch(newClassName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Class name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }
            classKB->removeClass(oldClassName);
            classKB->addClass(newClassName);
            loadDataFromBase(classKB, ui->classList);
        }
        return;
    }
    if (ui->tabWidget->currentIndex() == TAB_INDEX_PREDICATE)
    {
        QString oldPredicate = item->text();
        KnowledgeBasePredicateEditDialog *dialog = new KnowledgeBasePredicateEditDialog(oldPredicate, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newPredicate = dialog->predicateSign();
            QStringList definedPredicates = predicateKB->getData();

            // check the predicate sign in knowledge base
            if (definedPredicates.contains(newPredicate, Qt::CaseInsensitive))
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Predicate sign must be unique."));
                return;
            }

            // check the predicate name and arguments
            QStringList predicateSignList = newPredicate.split(" ");
            QString newPredicateName = predicateSignList[0];

            if ( newPredicateName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Predicate name can't be empty."));
                return;
            }

            if ( !nameChecker.exactMatch(newPredicateName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Predicate name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }

            for (int i = 1; i < predicateSignList.size(); i++)
            {
                if ( QString(predicateSignList[i]).isEmpty())
                {
                    QMessageBox::information(this, tr("KAVI"), tr("Argument name can't be empty."));
                    return;
                }

                if ( QString(predicateSignList[i]).toLower() == "object" )
                {
                    QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                    return;
                }

                if ( !nameChecker.exactMatch(predicateSignList[i]) )
                {
                    QMessageBox::warning(this, tr("KAVI"),
                    tr("Argument name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                    return;
                }
            }
            predicateKB->removePredicate(oldPredicate);
            predicateKB->addPredicate(newPredicate);
            loadDataFromBase(predicateKB, ui->predicateList);
        }
        return;
    }
}

void KnowledgeBaseEdit::on_buttonAdd_clicked()
{
    if (ui->tabWidget->currentIndex() == TAB_INDEX_CLASS)
    {
        KnowledgeBaseClassEditDialog *dialog = new KnowledgeBaseClassEditDialog(QString(), this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newClassName = dialog->className();
            QStringList definedClasses = classKB->getData();

            if ( newClassName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Class name can't be empty."));
                return;
            }

            if ( definedClasses.contains(newClassName, Qt::CaseInsensitive) )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Class name must be unique."));
                return;
            }

            if ( newClassName.toLower() == "object" )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                return;
            }

            if ( !nameChecker.exactMatch(newClassName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Class name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }
            classKB->addClass(newClassName);
            loadDataFromBase(classKB, ui->classList);
        }
        return;
    }
    if (ui->tabWidget->currentIndex() == TAB_INDEX_PREDICATE)
    {
        KnowledgeBasePredicateEditDialog *dialog = new KnowledgeBasePredicateEditDialog(QString(), this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newPredicate = dialog->predicateSign();
            QStringList definedPredicates = predicateKB->getData();

            // check the predicate sign in knowledge base
            if (definedPredicates.contains(newPredicate, Qt::CaseInsensitive))
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Predicate sign must be unique."));
                return;
            }

            // check the predicate name and arguments
            QStringList predicateSignList = newPredicate.split(" ");
            QString newPredicateName = predicateSignList[0];

            if ( newPredicateName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Predicate name can't be empty."));
                return;
            }

            if ( !nameChecker.exactMatch(newPredicateName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Predicate name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }

            for (int i = 1; i < predicateSignList.size(); i++)
            {
                if ( QString(predicateSignList[i]).isEmpty())
                {
                    QMessageBox::information(this, tr("KAVI"), tr("Argument name can't be empty."));
                    return;
                }

                if ( QString(predicateSignList[i]).toLower() == "object" )
                {
                    QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                    return;
                }

                if ( !nameChecker.exactMatch(predicateSignList[i]) )
                {
                    QMessageBox::warning(this, tr("KAVI"),
                    tr("Argument name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                    return;
                }
            }
            predicateKB->addPredicate(newPredicate);
            loadDataFromBase(predicateKB, ui->predicateList);
        }
        return;
    }
}

void KnowledgeBaseEdit::on_buttonEdit_clicked()
{

    if (ui->tabWidget->currentIndex() == TAB_INDEX_CLASS)
    {
        if (ui->classList->selectedItems().count() <= 0)
        {
            qDebug() << "@Please select one item";
            return;
        }

        QString oldClassName = ui->classList->currentItem()->text();
        KnowledgeBaseClassEditDialog *dialog = new KnowledgeBaseClassEditDialog(oldClassName, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newClassName = dialog->className();
            QStringList definedClasses = classKB->getData();

            if ( newClassName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Class name can't be empty."));
                return;
            }

            if ( definedClasses.contains(newClassName, Qt::CaseInsensitive) )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Class name must be unique."));
                return;
            }

            if ( newClassName.toLower() == "object" )
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                return;
            }

            if ( !nameChecker.exactMatch(newClassName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Class name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }
            classKB->removeClass(oldClassName);
            classKB->addClass(newClassName);
            loadDataFromBase(classKB, ui->classList);
        }
        return;
    }
    if (ui->tabWidget->currentIndex() == TAB_INDEX_PREDICATE)
    {
        if (ui->predicateList->selectedItems().count() <= 0)
        {
            qDebug() << "@Please select one item";
            return;
        }

        QString oldPredicate = ui->predicateList->currentItem()->text();
        KnowledgeBasePredicateEditDialog *dialog = new KnowledgeBasePredicateEditDialog(oldPredicate, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            QString newPredicate = dialog->predicateSign();
            QStringList definedPredicates = predicateKB->getData();

            // check the predicate sign in knowledge base
            if (definedPredicates.contains(newPredicate, Qt::CaseInsensitive))
            {
                QMessageBox::warning(this, tr("KAVI"), tr("Predicate sign must be unique."));
                return;
            }

            // check the predicate name and arguments
            QStringList predicateSignList = newPredicate.split(" ");
            QString newPredicateName = predicateSignList[0];

            if ( newPredicateName.isEmpty() )
            {
                QMessageBox::information(this, tr("KAVI"), tr("Predicate name can't be empty."));
                return;
            }

            if ( !nameChecker.exactMatch(newPredicateName) )
            {
                QMessageBox::warning(this, tr("KAVI"),
                tr("Predicate name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                return;
            }

            for (int i = 1; i < predicateSignList.size(); i++)
            {
                if ( QString(predicateSignList[i]).isEmpty())
                {
                    QMessageBox::information(this, tr("KAVI"), tr("Argument name can't be empty."));
                    return;
                }

                if ( QString(predicateSignList[i]).toLower() == "object" )
                {
                    QMessageBox::warning(this, tr("KAVI"), tr("Name \"object\" is reserved."));
                    return;
                }

                if ( !nameChecker.exactMatch(predicateSignList[i]) )
                {
                    QMessageBox::warning(this, tr("KAVI"),
                    tr("Argument name has wrong format.\n- only letters, digits, \"-\" and \"_\" are allowed\n- max lenght is limited\n- must start with letter"));
                    return;
                }
            }
            predicateKB->removePredicate(oldPredicate);
            predicateKB->addPredicate(newPredicate);
            loadDataFromBase(predicateKB, ui->predicateList);
        }
        return;
    }
}

void KnowledgeBaseEdit::on_buttonDelete_clicked()
{
    if (ui->tabWidget->currentIndex() == TAB_INDEX_CLASS)
    {
        if (ui->classList->selectedItems().count() <= 0)
        {
            qDebug() << "@Please select one item";
            return;
        }
        QString oldClassName = ui->classList->currentItem()->text();
        classKB->removeClass(oldClassName);
        loadDataFromBase(classKB, ui->classList);
        // this method does not work
        //ui->classList->removeItemWidget(ui->classList->currentItem());
        return;
    }
    if (ui->tabWidget->currentIndex() == TAB_INDEX_PREDICATE)
    {
        if (ui->predicateList->selectedItems().count() <= 0)
        {
            qDebug() << "@Please select one item";
            return;
        }
        QString oldPredicate = ui->predicateList->currentItem()->text();
        predicateKB->removePredicate(oldPredicate);
        loadDataFromBase(predicateKB, ui->predicateList);
    }
}

void KnowledgeBaseEdit::on_buttonCancel_clicked()
{
    this->reject();
}

void KnowledgeBaseEdit::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case TAB_INDEX_CLASS:
        loadDataFromBase(classKB, ui->classList);
        break;
    case TAB_INDEX_PREDICATE:
        loadDataFromBase(predicateKB, ui->predicateList);
        break;
    default:
        break;
    }
}
