/*
This file is part of Telegram Desktop Matzu Mod.
*/
#include "settings/sections/settings_matzu_pro.h"

#include "core/matzu_settings.h"
#include "settings/sections/settings_main.h"
#include "settings/settings_builder.h"
#include "settings/settings_common.h"
#include "settings/settings_common_session.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "window/window_session_controller.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
namespace {

using namespace Builder;

class MatzuPro : public Section<MatzuPro> {
public:
	MatzuPro(
		QWidget *parent,
		not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

private:
	void setupContent();
};

MatzuPro::MatzuPro(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section<MatzuPro>(parent, controller) {
	setupContent();
}

rpl::producer<QString> MatzuPro::title() {
	return rpl::single(QString("PRO Mode by Matzu"));
}

void MatzuPro::setupContent() {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

	const SectionBuildMethod buildMethod = [](
			not_null<Ui::VerticalLayout*> container,
			not_null<Window::SessionController*> controller,
			Fn<void(Type)> showOther,
			rpl::producer<> showFinished) {
		auto &lifetime = container->lifetime();
		const auto highlights = lifetime.make_state<HighlightRegistry>();
		const auto isPaused = Window::PausedIn(
			controller,
			Window::GifPauseReason::Layer);
		auto builder = SectionBuilder(WidgetContext{
			.container = container,
			.controller = controller,
			.showOther = std::move(showOther),
			.isPaused = isPaused,
			.highlights = highlights,
		});

		builder.addSubsectionTitle(rpl::single(QString("Master Switch")));

		// Master Toggle Switch
		builder.addCheckbox({
			.id = u"matzu/master"_q,
			.title = rpl::single(QString("Enable PRO Mode by Matzu")),
			.checked = Matzu::Config().proMode(),
		})->checkedChanges(
		) | rpl::start_with_next([](bool checked) {
			Matzu::Config().setProMode(checked);
		}, lifetime);

		builder.addDividerText(rpl::single(
			QString("Turn ON to activate all Ghost Mode, Silent View-Once Media Download, and Anti-Delete features.")
		));

		// Scope visible only when Master switch is ON
		builder.scope([&] {
			builder.addSkip();
			builder.addSubsectionTitle(rpl::single(QString("👻 Privacy & Ghost Mode")));

			// Ghost Hide Read
			builder.addCheckbox({
				.id = u"matzu/ghost_read"_q,
				.title = rpl::single(QString("Hide Read / Seen (Ghost Mode)")),
				.checked = Matzu::Config().ghostHideRead(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideRead(checked);
			}, lifetime);

			// Hide Typing
			builder.addCheckbox({
				.id = u"matzu/ghost_typing"_q,
				.title = rpl::single(QString("Hide Typing & Recording Status")),
				.checked = Matzu::Config().ghostHideTyping(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideTyping(checked);
			}, lifetime);

			// Hide Online Status
			builder.addCheckbox({
				.id = u"matzu/ghost_online"_q,
				.title = rpl::single(QString("Hide Online Status")),
				.checked = Matzu::Config().ghostHideOnline(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setGhostHideOnline(checked);
			}, lifetime);

			// Anti-Delete
			builder.addCheckbox({
				.id = u"matzu/anti_delete"_q,
				.title = rpl::single(QString("Anti-Delete Messages (Keep Deleted)")),
				.checked = Matzu::Config().antiDeleteMessages(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setAntiDeleteMessages(checked);
			}, lifetime);

			// Anti-Edit
			builder.addCheckbox({
				.id = u"matzu/anti_edit"_q,
				.title = rpl::single(QString("Anti-Edit Messages (Save Edit History)")),
				.checked = Matzu::Config().antiEditMessages(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setAntiEditMessages(checked);
			}, lifetime);

			builder.addDivider();
			builder.addSkip();
			builder.addSubsectionTitle(rpl::single(QString("🎬 Media & Security Tweaks")));

			// Save View-Once Media
			builder.addCheckbox({
				.id = u"matzu/save_view_once"_q,
				.title = rpl::single(QString("Save 1-View (View-Once) Media Silently")),
				.checked = Matzu::Config().saveViewOnceMedia(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setSaveViewOnceMedia(checked);
			}, lifetime);

			// No Forward Tag
			builder.addCheckbox({
				.id = u"matzu/no_forward_tag"_q,
				.title = rpl::single(QString("Forward Without Sender Tag")),
				.checked = Matzu::Config().noForwardTag(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setNoForwardTag(checked);
			}, lifetime);

			builder.addDivider();
			builder.addSkip();
			builder.addSubsectionTitle(rpl::single(QString("⚡ Productivity & Limits")));

			// Confirm Before Send
			builder.addCheckbox({
				.id = u"matzu/confirm_send"_q,
				.title = rpl::single(QString("Confirm Before Sending Voice & Calls")),
				.checked = Matzu::Config().confirmBeforeSend(),
			})->checkedChanges(
			) | rpl::start_with_next([](bool checked) {
				Matzu::Config().setConfirmBeforeSend(checked);
			}, lifetime);

			builder.addDividerText(rpl::single(
				QString("Unlimited Multi-Account Support (up to 100 accounts) is permanently unlocked.")
			));
		}, Matzu::Config().proModeValue());

		std::move(showFinished) | rpl::on_next([=] {
			for (const auto &[id, entry] : *highlights) {
				if (entry.widget) {
					controller->checkHighlightControl(
						id,
						entry.widget,
						base::duplicate(entry.args));
				}
			}
		}, lifetime);
	};

	build(content, buildMethod);
	Ui::ResizeFitChild(this, content);
}

const auto kMeta = BuildHelper({
	.id = MatzuPro::Id(),
	.parentId = MainId(),
	.title = &tr::lng_settings_section_privacy,
	.icon = &st::menuIconManage,
}, [](SectionBuilder &builder) {
});

} // namespace

Type MatzuProId() {
	return MatzuPro::Id();
}

} // namespace Settings
