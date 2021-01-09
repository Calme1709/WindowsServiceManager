export { default as default } from "./addon";

export enum ServiceType {
	/**
	 * The service is a device driver.
	 */
	KERNEL_DRIVER = 0x1,

	/**
	 * The service is a file system driver.
	 */
	FILE_SYSTEM_DRIVER = 0x2,

	/**
	 * The service runs in its own process.
	 */
	WIN32_OWN_PROCESS = 0x10,

	/**
	 * The service shares a process with other services.
	 */
	WIN32_SHARE_PROCESS = 0x20,

	/**
	 * The service runs in its own process under the logged-on user account.
	 */
	USER_OWN_PROCESS = 0x50,

	/**
	 * The service shares a process with one or more other services that run under the logged-on user account.
	 */
	USER_SHARE_PROCESS = 0x60,

	/**
	 * The service can interact with the desktop.
	 */
	INTERACTIVE_PROCESS = 0x100
}

export enum ServiceStartType {
	/**
	 * Indicates that the service is a device driver started by the system loader. This value is valid only for device drivers.
	 */
	BOOT = 0x0,

	/**
	 * Indicates that the service is a device driver started by the IOInitSystem function. This value is valid only for device drivers.
	 */
	SYSTEM = 0x1,

	/**
	 * Indicates that the service is to be started (or was started) by the operating system, at system start-up. If an automatically started service depends on a manually started service, the manually started service is also started automatically at system startup.
	 */
	AUTOMATIC = 0x2,

	/**
	 * Indicates that the service is started only manually, by a user (using the Service Control Manager) or by an application.
	 */
	MANUAL = 0x3,

	/**
	 * Indicates that the service is disabled, so that it cannot be started by a user or application.
	 */
	DISABLED = 0x4
}

export enum ServiceStatus {
	/**
	 * The service is not running.
	 */
	STOPPED = 0x1,

	/**
	 * The service is starting.
	 */
	START_PENDING = 0x2,

	/**
	 * The service is stopping.
	 */
	STOP_PENDING = 0x3,
	
	/**
	 * The service is running.
	 */
	RUNNING = 0x4,

	/**
	 * The service continue is pending.
	 */
	CONTINUE_PENDING = 0x5,

	/**
	 * The service pause is pending.
	 */
	PAUSE_PENDING = 0x6,

	/**
	 * The service is paused.
	 */
	PAUSED = 0x7,
}