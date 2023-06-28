/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "knewpasswordwidgettest.h"

#include <QAction>
#include <QLineEdit>
#include <QTest>

#include <KPasswordLineEdit>
#include <knewpasswordwidget.h>

QTEST_MAIN(KNewPasswordWidgetTest)

void KNewPasswordWidgetTest::testEmptyPasswordAllowed()
{
    KNewPasswordWidget pwdWidget;

    QVERIFY(pwdWidget.allowEmptyPasswords());
    QCOMPARE(pwdWidget.minimumPasswordLength(), 0);
    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::WeakPassword);
}

void KNewPasswordWidgetTest::testEmptyPasswordNotAllowed()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setAllowEmptyPasswords(false);

    QVERIFY(!pwdWidget.allowEmptyPasswords());
    QCOMPARE(pwdWidget.minimumPasswordLength(), 1);
    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::EmptyPasswordNotAllowed);
}

void KNewPasswordWidgetTest::testPasswordTooShort()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setMinimumPasswordLength(5);
    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    const QString password = QStringLiteral("1234");
    // We can't use setPassword here as when we call it for security we don't allow to show password. So we need to use setText
    linePassword->lineEdit()->setText(password);
    lineVerifyPassword->setText(password);

    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::PasswordTooShort);
}

void KNewPasswordWidgetTest::testPasswordMatch()
{
    KNewPasswordWidget pwdWidget;

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    const QString password = QStringLiteral("1234");
    linePassword->lineEdit()->setText(password);
    lineVerifyPassword->setText(password);

    QVERIFY(pwdWidget.passwordStatus() != KNewPasswordWidget::PasswordNotVerified);
    QCOMPARE(pwdWidget.password(), password);
}

void KNewPasswordWidgetTest::testPasswordNotVerified()
{
    KNewPasswordWidget pwdWidget;

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));

    QVERIFY(linePassword);

    const QString password = QStringLiteral("1234");
    linePassword->lineEdit()->setText(password);

    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::PasswordNotVerified);
}

void KNewPasswordWidgetTest::testWeakPassword()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setPasswordStrengthWarningLevel(30);
    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    const QString password = QStringLiteral("1234");
    linePassword->lineEdit()->setText(password);
    lineVerifyPassword->setText(password);

    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::WeakPassword);
}

void KNewPasswordWidgetTest::testStrongPassword()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setPasswordStrengthWarningLevel(99);
    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    const auto password = QStringLiteral("DHlKOJ1GotXWVE_fnqm1"); // generated by KeePass
    linePassword->lineEdit()->setText(password);
    lineVerifyPassword->setText(password);

    QCOMPARE(pwdWidget.passwordStatus(), KNewPasswordWidget::StrongPassword);
}

void KNewPasswordWidgetTest::testReasonablePasswordLength()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setReasonablePasswordLength(10);
    QCOMPARE(pwdWidget.reasonablePasswordLength(), 10);

    pwdWidget.setReasonablePasswordLength(0);
    QCOMPARE(pwdWidget.reasonablePasswordLength(), 1);

    pwdWidget.setReasonablePasswordLength(pwdWidget.maximumPasswordLength() + 1);
    QCOMPARE(pwdWidget.reasonablePasswordLength(), pwdWidget.maximumPasswordLength());
}

void KNewPasswordWidgetTest::testPasswordStrengthWarningLevel()
{
    KNewPasswordWidget pwdWidget;

    pwdWidget.setPasswordStrengthWarningLevel(40);
    QCOMPARE(pwdWidget.passwordStrengthWarningLevel(), 40);

    pwdWidget.setPasswordStrengthWarningLevel(-1);
    QCOMPARE(pwdWidget.passwordStrengthWarningLevel(), 0);

    pwdWidget.setPasswordStrengthWarningLevel(100);
    QCOMPARE(pwdWidget.passwordStrengthWarningLevel(), 99);
}

void KNewPasswordWidgetTest::testNoWarningColorBeforeMismatch()
{
    KNewPasswordWidget pwdWidget;
    QColor defaultColor = pwdWidget.palette().color(QPalette::Base);
    QColor warningColor(Qt::red);

    pwdWidget.setBackgroundWarningColor(warningColor);

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), defaultColor);

    lineVerifyPassword->setText(QStringLiteral("12"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), defaultColor);
}

void KNewPasswordWidgetTest::testWarningColorIfMismatch()
{
    KNewPasswordWidget pwdWidget;
    QColor defaultColor = pwdWidget.palette().color(QPalette::Base);
    QColor warningColor(Qt::red);

    pwdWidget.setBackgroundWarningColor(warningColor);

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), defaultColor);

    lineVerifyPassword->setText(QStringLiteral("122"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), warningColor);

    lineVerifyPassword->setText(QStringLiteral("1224"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), warningColor);
}

void KNewPasswordWidgetTest::testWarningColorPostMatch()
{
    KNewPasswordWidget pwdWidget;
    QColor defaultColor = pwdWidget.palette().color(QPalette::Base);
    QColor warningColor(Qt::red);

    pwdWidget.setBackgroundWarningColor(warningColor);

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword);

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    lineVerifyPassword->setText(QStringLiteral("1234"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), defaultColor);

    lineVerifyPassword->setText(QStringLiteral("12345"));
    QCOMPARE(lineVerifyPassword->palette().color(QPalette::Base), warningColor);
}

void KNewPasswordWidgetTest::disablingWidgetShouldUseDisabledPalette()
{
    KNewPasswordWidget pwdWidget;

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword && linePassword->isEnabled());
    QVERIFY(lineVerifyPassword && lineVerifyPassword->isEnabled());

    pwdWidget.setEnabled(false);

    QVERIFY(!linePassword->isEnabled());
    QVERIFY(!lineVerifyPassword->isEnabled());

    QCOMPARE(linePassword->palette(), pwdWidget.palette());
    QCOMPARE(lineVerifyPassword->palette(), pwdWidget.palette());
}

void KNewPasswordWidgetTest::disablingParentShouldUseDisabledPalette()
{
    auto widget = new QWidget();
    widget->setEnabled(false);

    auto pwdWidget = new KNewPasswordWidget(widget);
    QVERIFY(!pwdWidget->isEnabled());

    auto linePassword = pwdWidget->findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget->findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));

    QVERIFY(linePassword && !linePassword->isEnabled());
    QVERIFY(lineVerifyPassword && !lineVerifyPassword->isEnabled());

    QCOMPARE(linePassword->palette(), widget->palette());
    QCOMPARE(lineVerifyPassword->palette(), widget->palette());

    delete widget;
}

void KNewPasswordWidgetTest::disablingRevealPasswordShouldHideVisibilityAction()
{
    KNewPasswordWidget pwdWidget;

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    QVERIFY(linePassword);

    auto visibilityAction = linePassword->findChild<QAction *>(QStringLiteral("visibilityAction"));
    QVERIFY(visibilityAction && !visibilityAction->isVisible());

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    QVERIFY(visibilityAction->isVisible());
    QCOMPARE(pwdWidget.isRevealPasswordAvailable(), visibilityAction->isVisible());

    pwdWidget.setRevealPasswordAvailable(false);
    QVERIFY(!visibilityAction->isVisible());
    QCOMPARE(pwdWidget.isRevealPasswordAvailable(), visibilityAction->isVisible());
}

void KNewPasswordWidgetTest::shouldNotHideVisibilityActionInPlaintextMode()
{
    KNewPasswordWidget pwdWidget;

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    QVERIFY(linePassword);

    auto visibilityAction = linePassword->findChild<QAction *>(QStringLiteral("visibilityAction"));
    QVERIFY(visibilityAction && !visibilityAction->isVisible());

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    QVERIFY(visibilityAction->isVisible());

    visibilityAction->trigger();
    linePassword->clear();
    QVERIFY(visibilityAction->isVisible());
}

void KNewPasswordWidgetTest::shouldHideVerificationLineEditInPlaintextMode()
{
    KNewPasswordWidget pwdWidget;
    pwdWidget.show();

    auto linePassword = pwdWidget.findChild<KPasswordLineEdit *>(QStringLiteral("linePassword"));
    auto lineVerifyPassword = pwdWidget.findChild<QLineEdit *>(QStringLiteral("lineVerifyPassword"));
    QVERIFY(linePassword);
    QVERIFY(lineVerifyPassword && lineVerifyPassword->isVisible());

    auto visibilityAction = linePassword->findChild<QAction *>(QStringLiteral("visibilityAction"));
    QVERIFY(visibilityAction);

    linePassword->lineEdit()->setText(QStringLiteral("1234"));
    visibilityAction->trigger();
    QVERIFY(!lineVerifyPassword->isVisible());
}

#include "moc_knewpasswordwidgettest.cpp"
